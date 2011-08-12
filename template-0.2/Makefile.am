SUBDIRS = src

TEMPLATE       = template$(EXEEXT)

CLEANFILES = $(TEMPLATE)

EXTRA_DIST = autogen.sh share/ share/assets

all-local: $(TEMPLATE)

$(TEMPLATE):
	$(LN_S) "src/$(TEMPLATE)" "$(TEMPLATE)"

