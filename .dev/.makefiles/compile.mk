# Universal MakeFile v1.4
# Compatible with Code::blocks
include .makefiles/settings.mk

#Defauilt
all: $(OBJECTS) .makefiles/settings.mk


# dependency files (*.d) if available
#-include $(DEPENDS)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@echo -e $(TAB)$(BULLET2)$< -\> $@
	@$(MD) -p $(dir $@)
	@$(CC) $(COMFLAGS) $(INCDIRS) -c $< -o $@

#Non-File Targets
.PHONY: all
