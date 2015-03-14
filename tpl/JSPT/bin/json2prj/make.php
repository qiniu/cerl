<?php
{
	$PRJCONV_BITS = getenv("PRJCONV_BITS");
	$PRJCONV_OS = getenv("PRJCONV_OS");

	$plat = $doc->platform;
	if (!$PRJCONV_BITS)
		$PRJCONV_BITS = $plat->bits;
	if (!$PRJCONV_OS)
		$PRJCONV_OS = $plat->os;

	if ($plat->byteswap == "true")
		$platflags .= " -D__BYTESWAP__ -DBYTESWAP";

	if ($PRJCONV_BITS == "32")
		$platflags .= " -D__32BIT__";
	else if ($PRJCONV_BITS == "64")
		$platflags .= " -D__64BIT__";

	if ($plat->cpu == "intel")
		$platflags .= " -D__INTEL__";
	else if ($plat->cpu == "sparc")
		$platflags .= " -D__SPARC__";

	if ($PRJCONV_OS == "linux") {
		$platflags .= " -D__LINUX__ -DLINUX";
		$lib_prefix = "lib";
		$lib_suffix = ".a";
		$dll_suffix = ".so";
		$lib_linker = "@ar cr $(Product)";
		$dll_linker = "@g++ $(LibDir) -W1 -shared -o $(Product) -Wl,--version-script,_export_.def";
		$exe_linker = "@g++ $(LibDir) -o $(Product)";
		$check_link = "@ldd -u -r $(Product); echo";
		$os_abbr	= "li";
	}
	else if ($PRJCONV_OS == "windows") {
		$platflags .= " -D__WINDOWS__";
		$exe_suffix = ".exe";
		$lib_suffix = ".lib";
		$dll_suffix = ".dll";
		$os_abbr	= "win";
		die("Unsupported OS: windows");
	}
	else if ($PRJCONV_OS == "solaris") {
		$platflags .= " -D__SOLARIS__ -D__SOLS__ -DSOLARIS";
		$lib_prefix = "lib";
		$lib_suffix = ".a";
		$dll_suffix = ".so";
		$lib_linker = "@ar cr $(Product)";
		$dll_linker = "@CC -G -KPIC -O $(LibDir) -o $(Product)";
		$exe_linker = "@CC $(LibDir) -o $(Product)";
		$check_link = "@ldd -u -r $(Product); echo";
	}
	else {
		die("Unsupported OS: " . $plat->os);
	}
}
?>
<?php if ($PRJCONV_BITS != "independent") { ?>
OSAbbr	 = <?php echo "$os_abbr\n" ?>
Bits	 = <?php echo "$PRJCONV_BITS\n" ?>
Platform = <?php echo "$os_abbr$PRJCONV_BITS\n" ?>

<?php } ?>
#
# Configuration Default
#
ifeq ($(CFG),)
	CFG = Debug
endif

IncludeDir =<?php foreach ($doc->include as $inc) echo " -I$inc"; ?>

<?php
foreach ($doc->config as $cfg)
{
	$type = $cfg->product->type;
	$product = $cfg->product->path;
	$product_dir = dirname($product);
	$product_name = pathinfo($product, PATHINFO_BASENAME);
	$product_name_ext = strstr($product_name, ".");

	if (strcasecmp($type, "exe") == 0) {
		if (!$product_name_ext)
			$product = $product_dir . "/" . $product_name . $exe_suffix;
		$linker = $exe_linker;
	}
	else if (strcasecmp($type, "dll") == 0) {
		if (!$product_name_ext)
			$product = $product_dir . "/" . $lib_prefix . $product_name . $dll_suffix;
		$linker = $dll_linker;
	}
	else if (strcasecmp($type, "lib") == 0) {
		if (!$product_name_ext)
			$product = $product_dir . "/" . $lib_prefix . $product_name . $lib_suffix;
		$linker = $lib_linker;
	}
	else {
		die("Unknown product type: " . $type);
	}
	
	$objdir = $cfg->objdir;
	if ($objdir == null)
		$objdir = $product_dir;

	$cflags = "";
	$cxxflags = "";
	foreach ($cfg->option as $opt) {
		if ($opt == "Release")
			$cflags .= " -DNDEBUG -O2";
		else if ($opt == "Debug")
			$cflags .= " -D_DEBUG -g3";
		else if ($opt == "MultiThread")
			$cflags .= " -D_MT";
		else if ($opt == "Unicode")
			$cflags .= " -DUNICODE -D_UNICODE";
		else if ($opt == "AnsiChar")
			;
		else if ($opt == "Exception")
			$cflags .= " -fexceptions";
		else if ($opt == "WarningAll")
			$cflags .= " -Wall";
		else if ($opt == "NoRtti")
			$cxxflags .= " -fno-rtti";
		else if ($opt == "Rtti")
			$cxxflags .= " -frtti";
		else if ($opt == "MiniDepency")
			;
		else if ($opt{0} == "-")
			$cxxflags .= " " . $opt;
		else
			die("Unknown option: " . $opt);
	}
?>

#
# Configuration <?php echo $cfg->name . "\n" ?>
#
ifeq ($(CFG), <?php echo $cfg->name; ?>)
<?php
	if ($cfg->define)
		foreach ($cfg->define as $define)
			echo "	Defines	    += -D$define\n";
	if ($cfg->libdir)
		foreach ($cfg->libdir as $libdir)
			echo "	LibDir	    += -L$libdir\n";
	if ($cfg->lib)
		foreach ($cfg->lib as $lib)
			echo "	Libraries   += -l$lib\n";
?>
	ProductDir   = <?php echo "$product_dir\n" ?>
	ObjectDir    = <?php echo "$objdir\n" ?>
	Product      = <?php echo "$product\n" ?>
	CFlags       = $(Defines) -fPIC<?php echo "$platflags$cflags\n" ?>
	CXXFlags     = $(CFlags)<?php echo "$cxxflags\n" ?>
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @g++ -c $(IncludeDir) $(CXXFlags) -o $@ $<
	Link         = <?php echo "$linker\n" ?>
	CheckLink    = <?php echo "$check_link\n" ?>
	LD_LIBRARY_PATH += :$(ProductDir)
endif
<?php } ?>

#
# Making...
#
all : $(ProductDir) $(ObjectDir) StartMake $(Product) EndMake

$(ProductDir) :
	@mkdir -p $(ProductDir)

ifneq ($(ProductDir),$(ObjectDir))
$(ObjectDir) :
	@mkdir -p $(ObjectDir)
endif

clean :
	@rm -f $(ObjectDir)/*.o
	@rm -f $(Product)

StartMake :
	@echo "-------------- Configuration:" $(PROJ) $(CFG) "--------------"
ifeq ($(ForceLink),1)
	@rm -f $(Product)
endif

EndMake :
	@echo ">>> Success <<<"

#
# Source Files, Objects & Product
#
<?php foreach ($doc->src as $src_item) {
	if (strstr($src_item, "*"))
		$srclist = glob($src_item);
	else
		$srclist = array($src_item);
	foreach ($srclist as $src) {
		$obj = "$(ObjectDir)/" . pathinfo($src, PATHINFO_FILENAME) . ".o ";
		$objs .= $obj;
		if (pathinfo($src, PATHINFO_EXTENSION) == "c")
			$compile = "$(CompileC)";
		else
			$compile = "$(CompileCXX)";
?>
<?php echo $obj ?>:	\
	<?php echo "$src\n" ?>
	@echo "---> Compiling" $< "..."
	<?php echo "$compile\n" ?>
<?php } } ?>

Objects = <?php echo "$objs\n" ?>

$(Product) : $(Objects)
	@echo "---> Creating" $@ "..."
	@rm -f $@
	$(Link) $(Objects) $(Libraries)
	$(CheckLink)

