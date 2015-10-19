###########################################
# 1. ����actionsconfig.mk����Ŀ¼��
# 2. ����actionsconfig.java��framework/base/actions/com/actions
# 3. ����actionsconfig.h��bionic/libc/include
#
###########################################

.PHONY: gen_actionsconfig

ACTIONC_CONFIG_MK_PATH_IPUT =build/actions_owl.config
ACTIONC_CONFIG_MK_PATH =actionsconfig.mk
ACTIONC_CONFIG_JAVA_PATH =frameworks/base/actions/com/actions/
ACTIONC_CONFIG_H_PATH =bionic/libc/include/


$(info  gen_actionsconfig  processs...........)

gen_actionsconfig:
	@echo gen_actionsconfig
	mkdir -p $(ACTIONC_CONFIG_JAVA_PATH)
	mkdir -p $(ACTIONC_CONFIG_H_PATH)
	build/tools/gen_actions_config.sh $(ACTIONC_CONFIG_MK_PATH_IPUT) \
	$(ACTIONC_CONFIG_JAVA_PATH)/ActionsConfig.java  $(ACTIONC_CONFIG_H_PATH)/actionsconfig.h
	