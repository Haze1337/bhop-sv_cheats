# sv_cheats Spoofer (CS:S Only)
The plugin spoofs `sv_cheats` ConVar, changes the name, removes the flags and creates a new `sv_cheats` ConVar to bypass the check via sourcemod.

To use it, you need to add `-insecure` to the launch parameters.

## Example

![0](/screenshot/example.png)

The first ConVar(`sv_cheats`) does nothing, but when `sv_cheats` is changed by the server, it will also change the value.  
The second one(`sv_cheats_xxxxx`) works like a regular `sv_cheats`, but you can change the value at any time and use cheat commands.