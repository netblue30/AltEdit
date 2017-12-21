#!/bin/bash

astyle --options=none --lineend=linux --mode=c --style=java \
--attach-classes --break-closing-brackets --indent=force-tab=8 \
--indent-preprocessor --min-conditional-indent=0 \
--pad-oper --pad-comma --align-pointer=name \
--align-reference=name --break-after-logical --suffix=none --quiet < $1
