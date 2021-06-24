project(
	'C+ preprocessor', 'c',
	version: '0.0.2',
	license: 'GPL-3.0-or-later',
	default_options: ['c_std=c11', 'warning_level=3']
)
# disable strict aliasning
if meson.get_compiler('c').has_argument('-fno-strict-aliasing')
	extra_args = ['-fno-strict-aliasing']
endif
# include header files
inc = include_directories('include')
# include source files
subdir('src')
subdir('test')
