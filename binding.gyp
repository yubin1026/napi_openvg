{
	'targets': [
	{
		'target_name': 'napi_openvg',
		'sources': [
			'src/napi_openvg.c',
		],
		'conditions': [[
			'OS=="mac"', {
				'libraries': [
					'/usr/local/lib/libOpenVG.dylib',
				],
				'xcode_settings': {
            		'OTHER_LDFLAGS': [
              			'-undefined dynamic_lookup',
              			'-framework Cocoa',
              			'-framework OpenGL',              			
              			'-framework OpenAL',
            		]
          		},
				'include_dirs': [
					'/usr/local/include'
				]
			}
		]],
		'defines': [
			'_THREAD_SAFE',
			'ENABLE_ARG_CHECKING',
		]
	}
	]
}
