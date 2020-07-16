################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
block_sine.obj: ../block_sine.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.18/include" --include_path="C:/Users/Ian/Desktop/MSc Thesis/fftss-3.0/include" --include_path="C:/eXperimenter/c6748/bsl/inc" --include_path="C:/eXperimenter/L138_chapter2/GuitarAmpGitVersion/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="block_sine.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

framework.obj: ../framework.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.18/include" --include_path="C:/Users/Ian/Desktop/MSc Thesis/fftss-3.0/include" --include_path="C:/eXperimenter/c6748/bsl/inc" --include_path="C:/eXperimenter/L138_chapter2/GuitarAmpGitVersion/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="framework.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hello.obj: ../hello.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.18/include" --include_path="C:/Users/Ian/Desktop/MSc Thesis/fftss-3.0/include" --include_path="C:/eXperimenter/c6748/bsl/inc" --include_path="C:/eXperimenter/L138_chapter2/GuitarAmpGitVersion/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="hello.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-1887942079: ../hello.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/ti/bios_5_42_02_10/xdctools/tconf" -b -Dconfig.importPath="C:/ti/bios_5_42_02_10/packages;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hellocfg.cmd: build-1887942079
hellocfg.s??: build-1887942079
hellocfg_c.c: build-1887942079
hellocfg.h: build-1887942079
hellocfg.h??: build-1887942079
hello.cdb: build-1887942079

hellocfg.obj: ./hellocfg.s?? $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.18/include" --include_path="C:/Users/Ian/Desktop/MSc Thesis/fftss-3.0/include" --include_path="C:/eXperimenter/c6748/bsl/inc" --include_path="C:/eXperimenter/L138_chapter2/GuitarAmpGitVersion/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="hellocfg.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hellocfg_c.obj: ./hellocfg_c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.18/include" --include_path="C:/Users/Ian/Desktop/MSc Thesis/fftss-3.0/include" --include_path="C:/eXperimenter/c6748/bsl/inc" --include_path="C:/eXperimenter/L138_chapter2/GuitarAmpGitVersion/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="hellocfg_c.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ignore.obj: ../ignore.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c6000_7.4.18/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/ti/ccsv6/tools/compiler/c6000_7.4.18/include" --include_path="C:/Users/Ian/Desktop/MSc Thesis/fftss-3.0/include" --include_path="C:/eXperimenter/c6748/bsl/inc" --include_path="C:/eXperimenter/L138_chapter2/GuitarAmpGitVersion/Debug" --include_path="C:/ti/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/ti/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ignore.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


