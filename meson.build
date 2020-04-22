project('chipoch', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

sdl2 = meson.get_compiler('c').find_library('SDL2')
math = meson.get_compiler('c').find_library('m')

#chipoch_exe = executable('chipoch',
executable('chipoch',
           'video.c',
           'main.c',
           'chip-8.c',
           install : true,
	   install_dir : '/home/dmarfil/bin',
           dependencies: [sdl2, math])

#test('run', chipoch_exe)
#run_target('/home/dmarfil/projects/chip-8/builddir/chipoch')
#run_target('run', command : [chipoch_exe])
#run_target('run', command : [chipoch_exe, '/home/dmarfil/projects/chip-8/roms2/RandomNumberTestMatthewMikolay2010.ch8'])
 #, depends: chipoch_exe )


