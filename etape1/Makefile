EXEC      = TER
DEPDIR    = mtl
PWD        = $(shell pwd)

CSRCS      = $(wildcard $(PWD)/*.cc) 
DSRCS      = $(foreach dir, $(DEPDIR), $(filter-out $(MROOT)/$(dir)/Main.cc, $(wildcard $(MROOT)/$(dir)/*.cc)))
COBJS      = $(CSRCS:.cc=.o) $(DSRCS:.cc=.o)


CXX       ?= g++
CFLAGS    ?= -Wall -Wno-parentheses
LFLAGS    ?= -Wall

COPTIMIZE ?= -O3

CFLAGS    += -I$(MROOT) -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS
LFLAGS    += -lz -L. -lpatoh -lm 

.PHONY : s clean

s:	$(EXEC)


## Compile options
%.o:			CFLAGS +=$(COPTIMIZE) -g -D DEBUG

## Link options
$(EXEC):		LFLAGS += -g

## Dependencies
$(EXEC):		$(COBJS)

## Build rule
%.o :	%.cc
	@echo Compiling: $(subst $(MROOT)/,,$@)
	@$(CXX) $(CFLAGS) -c -o $@ $<

## Linking rules (standard/profile/debug/release)
$(EXEC):
	@echo Linking: "$@ ( $(foreach f,$^,$(subst $(MROOT)/,,$f)) )"
	@$(CXX) $^ $(LFLAGS) -o $@

## Clean rule
clean:
	@rm -f $(EXEC) $(EXEC)_static \
	  $(COBJS)*.core depend.mk 
	  
## Make dependencies
depend.mk: $(CSRCS)
	@echo Making dependencies
	@$(CXX) $(CFLAGS) -I$(MROOT) \
	   $(CSRCS) -MM | sed 's|\(.*\):|$(PWD)/\1 $(PWD)/\1r $(PWD)/\1d $(PWD)/\1p:|' > depend.mk
	@for dir in $(DEPDIR); do \
	      if [ -r $(MROOT)/$${dir}/depend.mk ]; then \
		  echo Depends on: $${dir}; \
		  cat $(MROOT)/$${dir}/depend.mk >> depend.mk; \
	      fi; \
	  done
-include depend.mk
