<h1 align=center>Latoru</h1>
<p align=center>Latoru is a tool that helps Russian learners type Russian using the Latin alphabet while the program replaces it with Cyrillic characters.</p>

## How it Works

We set a low level hook on the keyboard, and evaluate which buttons were 
pressed. If combinations such as "ye", "yu", "yo", "ya" were pressed, 
we use the "y" as a modifier and send out the correct character.

## Contributing

Any kind of contribution is welcome! (please fix my bad code)

## Issues

- [ ] Big ol' ugly switch-case that is responsible for the characters
- [ ] Output is in ALL CAPS (seriously, until this is fixed I'd only use this if ur friends are fine with you screaming at them)
- [ ] `ы`, `ь` and `ъ` don't work as expected :P
- [ ] I forgor to add the `щ` 💀
