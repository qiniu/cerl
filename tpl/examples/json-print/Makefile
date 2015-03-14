#
# Configuration Default
#
ifeq ($(CFG),)
	CFG = Debug
endif

IncludeDir = -I../../../tpl/include 

#
# Configuration Release
#
ifeq ($(CFG), Release)
	ProductDir   = Release
	ObjectDir    = Release
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = Release/simplest
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -DNDEBUG -O2 -D_MT -fexceptions 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -ldl -o $(Product)
	LD_LIBRARY_PATH += :$(ProductDir)
endif

#
# Configuration Debug
#
ifeq ($(CFG), Debug)
	ProductDir   = Debug
	ObjectDir    = Debug
	MocFile      = $(ObjectDir)/temp.moc.cpp
	Product      = Debug/simplest
	CFlags       = $(Defines) -fPIC -D__INTEL__ -D__LINUX__ -DLINUX -D_DEBUG -g3 -D_MT -fexceptions 
	CXXFlags     = $(CFlags) -fno-rtti 
	CXX          = g++ -c $(IncludeDir) $(CXXFlags) -o $@
	CompileC     = @gcc -c $(IncludeDir) $(CFlags) -o $@ $<
	CompileCXX   = @$(CXX) $<
	CompileMoc   = @moc $< -o $(MocFile) && $(CXX) $(MocFile) && rm -f $(MocFile)
	Link         = @g++ $(LibDir) -ldl -o $(Product)
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
$(ObjectDir)/Simplest.o :	\
	Simplest.cpp
	@echo "---> Compiling" $< "..."
	$(CompileCXX)


Objects =	\
	$(ObjectDir)/Simplest.o

$(Product) : $(Objects) $(Libs)
	@echo "---> Creating" $@ "..."
	@rm -f $@
	$(Link) $(Objects) $(Libraries)
	$(CheckLink)

