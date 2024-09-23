function(lsm_add_linker_flags)

set(LINKER_FLAGS
	-specs=nosys.specs
	-lc
	-lm
	-lnosys
	-Wl,-Map=project.map,--cref
	-Wl,--gc-sections
	-Wl,--print-memory-usage
)

target_link_options(${EXECUTABLE} PRIVATE
	-T${LSM_SOURCE_DIR}/Devices/${DEVICE}/linker.ld
	${CORE}
  -mthumb
	${LINKER_FLAGS}
)

endfunction()