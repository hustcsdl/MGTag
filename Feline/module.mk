local_src := $(wildcard $(subdirectory)/*.cpp)

$(eval $(call make-program,feline,libtriplebit.a,$(local_src)))

$(eval $(call compile-rules))
