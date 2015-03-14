#
# Configuration Default
#
ifeq ($(CFG),)
	CFG = Debug
endif

IncludeDir = -I../../../tpl/include -I../../../stdext/include -I../../../boost

#
# Configuration Release
#
ifeq ($(CFG), Release)
	ProductDir   = Release
	ObjectDir    = Release
	Product      = Release/lang
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -DNDEBUG -O2 -D_MT -fexceptions
	CXXFlags     = $(CFlags) -fno-rtti
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @g++ -c $(IncludeDir) $(CXXFlags) -o $@ $<
	Link         = @g++ $(LibDir) -o $(Product)
	CheckLink    = @ldd -u -r $(Product); echo
	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Configuration Debug
#
ifeq ($(CFG), Debug)
	ProductDir   = Debug
	ObjectDir    = Debug
	Product      = Debug/lang
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -D_DEBUG -g3 -D_MT -fexceptions
	CXXFlags     = $(CFlags) -fno-rtti
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @g++ -c $(IncludeDir) $(CXXFlags) -o $@ $<
	Link         = @g++ $(LibDir) -o $(Product)
	CheckLink    = @ldd -u -r $(Product); echo
	LD_LIBRARY_PATH += :$(ProductDir)
endif

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
$(ObjectDir)/Lang.o :	\
	Lang.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)

Objects = $(ObjectDir)/Lang.o 

$(Product) : $(Objects)
	@echo "---> Creating" $@ "..."
	@rm -f $@
	$(Link) $(Objects) $(Libraries)
	$(CheckLink)

