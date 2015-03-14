<?php
{
	require("include/guid.class.php");

	$plat = $doc->platform;

	$PRJCONV_BITS = getenv("PRJCONV_BITS");
	if (!$PRJCONV_BITS)
		$PRJCONV_BITS = $plat->bits;

	$VCPROJ_VERSION = getenv("VCPROJ_VERSION");
	if (!$VCPROJ_VERSION)
		$VCPROJ_VERSION = "8.00";

	if ($plat->byteswap == "true")
		$platdefine .= ";__BYTESWAP__;BYTESWAP";

	if ($PRJCONV_BITS == "32")
		$platdefine .= ";__32BIT__";
	else if ($PRJCONV_BITS == "64")
		$platdefine .= ";__64BIT__";

	$platform = "Win" . $PRJCONV_BITS;
	$prjname = pathinfo($doc->config[0]->product->path, PATHINFO_FILENAME);
	$exe_suffix = ".exe";
	$dll_suffix = ".dll";
	$lib_suffix = ".lib";
}
echo '<?xml version="1.0" encoding="gb2312"?>';
?>

<VisualStudioProject
	ProjectType="Visual C++"
	Version="<?php echo $VCPROJ_VERSION ?>"
	Name="<?php echo $prjname ?>"
	ProjectGUID="{<?php echo createGuid() ?>}"
	>
	<Platforms>
		<Platform
			Name="<?php echo $platform ?>"
		/>
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
<?php
foreach ($doc->config as $cfg)
{
	$search = array('/\$\(OSAbbr\)/', '/\$\(Bits\)/', '/\$\(Platform\)/');
	$replace = array('win', $PRJCONV_BITS, 'win'.$PRJCONV_BITS);
	$type = $cfg->product->type;
	$product = preg_replace($search, $replace, $cfg->product->path);
	$product_dir = dirname($product);
	$product_name = pathinfo($product, PATHINFO_BASENAME);
	$product_fname = pathinfo($product, PATHINFO_FILENAME);
	$product_name_ext = strstr($product_name, ".");
	$define = "WIN" . $PRJCONV_BITS . $platdefine;

	if (strcasecmp($type, "exe") == 0) {
		if (!$product_name_ext)
			$product = $product_dir . "/" . $product_name . $exe_suffix;
		$linker = $exe_linker;
		$define .= ";_CONSOLE";
		$type_enum = "1";
	}
	else if (strcasecmp($type, "dll") == 0) {
		if (!$product_name_ext)
			$product = $product_dir . "/" . $lib_prefix . $product_name . $dll_suffix;
		$linker = $dll_linker;
		$define .= ";_DLL";
		$type_enum = "2";
	}
	else if (strcasecmp($type, "lib") == 0) {
		if (!$product_name_ext)
			$product = $product_dir . "/" . $lib_prefix . $product_name . $lib_suffix;
		$linker = $lib_linker;
		$define .= ";_LIB";
		$type_enum = "3";
	}
	else {
		die("Unknown product type: " . $type);
	}
	
	$objdir = $cfg->objdir;
	if ($objdir == null)
		$objdir = $product_dir;

	$optimization = 2;
	$inline = 1;
	$debug = 0;
	$minidep = 0;
	
	if ($cfg->define)
		foreach ($cfg->define as $def)
			$define .= ";" . $def;

	foreach ($cfg->option as $opt) {
		if ($opt == "Release") {
			$define .= ";NDEBUG";
			$optimization = "2";
			$inline = "1";
			$debug = 0;
		}
		else if ($opt == "Debug") {
			$define .= ";_DEBUG";
			$optimization = "0";
			$inline = "0";
			$debug = 1;
		}
		else if ($opt == "MultiThread")
			$define .= ";_MT";
		else if ($opt == "Unicode")
			$define .= ";UNICODE;_UNICODE";
		else if ($opt == "AnsiChar")
			;
		else if ($opt == "Exception")
			;
		else if ($opt == "WarningAll")
			;
		else if ($opt == "NoRtti")
			;
		else if ($opt == "Rtti")
			;
		else if ($opt == "MiniDepency")
			$minidep = 1;
		else if ($opt{0} == "-")
			;
		else
			die("Unknown option: " . $opt);
	}
	if ($minidep)
		$crt_type = $debug ? "1" : "0";
	else
		$crt_type = $debug ? "3" : "2";
?>
		<Configuration
			Name="<?php echo $cfg->name . '|' . $platform ?>"
			OutputDirectory="<?php echo $product_dir ?>"
			IntermediateDirectory="<?php echo $objdir ?>"
			ConfigurationType="<?php echo $type_enum ?>"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="false"
			CharacterSet="2"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
				TypeLibraryName="<?php echo $objdir . '/' . $product_fname . '.tlb' ?>"
				HeaderFileName=""
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="<?php echo $optimization ?>"
				InlineFunctionExpansion="<?php echo $inline ?>"
				AdditionalIncludeDirectories="<?php if (isset($doc->include)) echo join(',', $doc->include); ?>"
				PreprocessorDefinitions="<?php echo $define ?>"
				StringPooling="true"
				RuntimeLibrary="<?php echo $crt_type ?>"
				EnableFunctionLevelLinking="true"
				PrecompiledHeaderFile=""
				AssemblerListingLocation="<?php echo $objdir . '/' ?>"
				ObjectFile="<?php echo $objdir . '/' ?>"
				ProgramDataBaseFileName="<?php echo $objdir . '/' ?>"
				WarningLevel="3"
				SuppressStartupBanner="true"
				DebugInformationFormat="<?php echo $debug ? '4' : '3' ?>"
				CompileAs="0"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="<?php echo $define ?>"
				Culture="2052"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				OutputFile="<?php echo $product ?>"
				LinkIncremental="1"
				SuppressStartupBanner="true"
				AdditionalLibraryDirectories="<?php if ($cfg->libdir) echo join(',', $cfg->libdir); ?>"
				GenerateDebugInformation="<?php echo $debug ? 'true' : 'false' ?>"
				ProgramDatabaseFile="<?php echo $objdir . '/' . $product_fname . '.pdb' ?>"
				SubSystem="1"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCWebDeploymentTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
<?php } ?>
	</Configurations>
	<References>
	</References>
	<Files>
<?php foreach ($doc->src as $src_item) {
	if (strstr($src_item, "*"))
		$srclist = glob($src_item);
	else
		$srclist = array($src_item);
	foreach ($srclist as $src) {
?>
		<File
			RelativePath="<?php echo str_replace('/', '\\', $src) ?>"
			>
		</File>
<?php } } ?>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>

