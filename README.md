# randomize-menu-music
 randomize gd menu music on startup

# usage
without mega hack 6:
- download the zip in releases
- drag both dlls into your Geometry Dash installation folder. replace the existing libtiff.dll with the downloaded one.
- open gd
- add mp3 files to the new folder named "menuLoops" in your installation folder.
- enjoy (?)

with mega hack 6:
- download the zip in releases
- drag the "randomize-menu-music.dll" file into your Geometry Dash installation folder.
- edit "absolutedlls" with a new line, that adds the name of the new file, "randomize-menu-music.dll". no quotes.
- open gd
- add mp3 files to the new folder named "menuLoops" in your installation folder.
- enjoy (?)

# usage (advanced)
- clone this repo.
- build the solution.
- open geometry dash.
- use a dll injector to inject the newly compiled dll OR
- use a portable executable header editor to edit an existing dependency of geometry dash to make the mod auto-inject.
- enjoy! you can play around with it as much as you'd like.
