# Compiler
MAVN-prevodilac predstavlja programski alat koji prevodi program napisan na višem MIPS 32bit programskom jeziku na osnovni asemblerski jezik. Viši MIPS 32bit asemblerski jezik 
služi lakšem asemblerskom programiranju jer uvodi koncept registarske promenljive koje omogućavaju programerima da prilikom pisanja programa koriste promenljive umesto pravih 
resursa, što u mnogome olakšava programiranje.  U ovom projektu se, radi jednostavnosti, ograničava funkcionalnost MIPS 32bit jezika na korišćenje isključivo promenljive.

MAVN jezik, čiji je prevodilac realizovan u ovom projektu, podržava 15 MIPS instrukcija, i to:
add – (addition) sabiranje
addi – (addition immediate) sabiranje sa konstantom
b – (unconditional branch) bezuslovni skok
bltz – (branch on less than zero) skok ako je registar manji od nule
la – (load address) učitavanje adrese u registar 
li – (load immediate) učitavanje konstante u registar
lw – (load word) učitavanje jedne memorijske reči
la – (no operation) instrukcija bez operacije
sub – (subtraction) oduzimanje
sw – (store word) upis jedne memorijske reči
and – (logical AND) izvrši logičku operaciju „i“ nad registrom
or – (logical OR) izvrši logičku operaciju „ili“ nad registrom
nor – (logical NOR) izvrši logičku operaciju „ekskluzivno ili / nili“ nad registrom
bgtz – (branch on greater than zero) skok ako je registar većii od nule
seq – (set equal) postavlja odredišni registar na 1 ako je prvi izvorišni registar jednak drugom, u suprotnom odredišni registar postavlja na 0
