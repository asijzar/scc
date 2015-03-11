use v5.18;
use strict;
use warnings;
use utf8;

my %machine;
$machine{chr($_)} = [0] foreach (0..32);
$machine{chr($_)} = [$_ - 32] foreach (33..126);

my $ftname = 'machine';
open(my $fit, '>', $ftname) or die "Can't open '$ftname'!";

my $fcname = 'machine.h';
open(my $fic, '>', $fcname) or die "Can't open '$fcname'!";

#Ending states
#30 - ! (unget)
#31 - !=
#32 - string constant
#33 - % (unget)
#34 - %=
#35 - & (unget)
#36 - &&
#37 - &=
#38 - char
#39 - (
#40 - )
#41 - * (unget)
#42 - *=
#43 - + (unget)
#44 - ++
#45 - +=
#46 - ,
#47 - - (unget)
#48 - ->
#49 - --
#50 - -=
#51 - . (unget)
#52 - / (unget)
#53 - /=
#54 - one-line comment
#55 - multi-line comment
#56 - octal constant (unget)
#57 - hex constant (unget)
#58 - decimal constant (unget)
#59 - float constant (unget)
#60 - :
#61 - ;
#62 - < (unget)
#63 - << (unget)
#64 - <=
#65 - <<=
#66 - = (unget)
#67 - ==
#68 - > (unget)
#69 - >> (unget)
#70 - >=
#71 - >>=
#72 - ?
#73 - identifier
#74 - [
#75 - ]
#76 - ^ (unget)
#77 - ^=
#78 - {
#79 - | (unget)
#80 = ||
#81 - |=
#82 - }
#83 - ~

#errors:
#84 - unrecognized token
#85 - invalid digit in octal constant
#86 - invalid suffix on integer constant
#87 - exponent has no digits

#int in [] - current state, char in {} - char that we met

#!(=)
push @{$machine{chr($_)}}, 30 foreach (0..126);
@{$machine{"="}}[1] = 31;

#string Literal
@{$machine{'"'}}[0] = 32;

##
@{$machine{"#"}}[0] = 84;

#$
@{$machine{"\$"}}[0] = 84;

#%(=)
@{$machine{"%"}}[0] = 2;
push @{$machine{chr($_)}}, 33 foreach (0..126);
@{$machine{"="}}[2] = 34;

#&(&, =)
@{$machine{"&"}}[0] = 3;
push @{$machine{chr($_)}}, 35 foreach (0..126);
@{$machine{"&"}}[3] = 36;
@{$machine{"="}}[3] = 37;

#char
@{$machine{"'"}}[0] = 38;

#(
@{$machine{"("}}[0] = 39;

#)
@{$machine{")"}}[0] = 40;

#*(=)
@{$machine{"*"}}[0] = 4;
push @{$machine{chr($_)}}, 41 foreach (0..126);
@{$machine{"="}}[4] = 42;

#+(+, =)
@{$machine{"+"}}[0] = 5;
push @{$machine{chr($_)}}, 43 foreach (0..126);
@{$machine{"+"}}[5] = 44;
@{$machine{"="}}[5] = 45;

#,
@{$machine{","}}[0] = 46;

#-(>, -, =)
@{$machine{"-"}}[0] = 6;
push @{$machine{chr($_)}}, 47 foreach (0..126);
@{$machine{">"}}[6] = 48;
@{$machine{"-"}}[6] = 49;
@{$machine{"="}}[6] = 50;

#.
@{$machine{"."}}[0] = 7;
push @{$machine{chr($_)}}, 51 foreach (0..126);
@{$machine{$_}}[7] = 17 foreach ("0".."9");

#/(=)
@{$machine{"/"}}[0] = 8;
push @{$machine{chr($_)}}, 52 foreach (0..126);
@{$machine{"="}}[8] = 53;
@{$machine{"/"}}[8] = 54;
@{$machine{"*"}}[8] = 55;

#octal constant
@{$machine{"0"}}[0] = 9;
push @{$machine{chr($_)}}, 58 foreach (0..126);
@{$machine{$_}}[9] = 10 foreach ("0".."7");
@{$machine{$_}}[9] = 85 foreach ("8".."9");
@{$machine{$_}}[9] = 86 foreach ("A".."Z");
@{$machine{$_}}[9] = 86 foreach ("a".."z");
push @{$machine{chr($_)}}, 56 foreach (0..126);
@{$machine{$_}}[10] = 10 foreach ("0".."7");
@{$machine{$_}}[10] = 85 foreach ("8".."9");
@{$machine{$_}}[10] = 86 foreach ("A".."Z");
@{$machine{$_}}[10] = 86 foreach ("a".."z");

#hex constant
@{$machine{"x"}}[9] = 11;
@{$machine{"X"}}[9] = 11;
push @{$machine{chr($_)}}, 86 foreach (0..126);
@{$machine{$_}}[11] = 12 foreach ("0".."9");
@{$machine{$_}}[11] = 12 foreach ("A".."F");
@{$machine{$_}}[11] = 12 foreach ("a".."f");
push @{$machine{chr($_)}}, 57 foreach (0..126);
@{$machine{$_}}[12] = 12 foreach ("0".."9");
@{$machine{$_}}[12] = 12 foreach ("A".."F");
@{$machine{$_}}[12] = 12 foreach ("a".."f");
@{$machine{$_}}[12] = 86 foreach ("G".."Z");
@{$machine{$_}}[12] = 86 foreach ("g".."z");

#decimal constant
@{$machine{$_}}[0] = 13 foreach ("1".."9");
push @{$machine{chr($_)}}, 58 foreach (0..126);
@{$machine{$_}}[13] = 13 foreach ("0".."9");
@{$machine{"e"}}[13] = 14;
@{$machine{"E"}}[13] = 14;
push @{$machine{chr($_)}}, 87 foreach (0..126);
@{$machine{$_}}[14] = 16 foreach ("0".."9");
@{$machine{"+"}}[14] = 15;
@{$machine{"-"}}[14] = 15;
push @{$machine{chr($_)}}, 87 foreach (0..126);
@{$machine{$_}}[15] = 16 foreach ("0".."9");
push @{$machine{chr($_)}}, 58 foreach (0..126);
@{$machine{$_}}[16] = 16 foreach ("0".."9");

#floating constant
@{$machine{"."}}[9] = 17;
@{$machine{"."}}[10] = 17;
@{$machine{"."}}[13] = 17;
push @{$machine{chr($_)}}, 59 foreach (0..126);
@{$machine{$_}}[17] = 17 foreach ("0".."9");
@{$machine{"e"}}[17] = 18;
@{$machine{"E"}}[17] = 18;
push @{$machine{chr($_)}}, 87 foreach (0..126);
@{$machine{$_}}[18] = 20 foreach ("0".."9");
@{$machine{"+"}}[18] = 19;
@{$machine{"-"}}[18] = 19;
push @{$machine{chr($_)}}, 87 foreach (0..126);
@{$machine{$_}}[19] = 20 foreach ("0".."9");
push @{$machine{chr($_)}}, 59 foreach (0..126);
@{$machine{$_}}[20] = 20 foreach ("0".."9");

#:
@{$machine{":"}}[0] = 60;

#;
@{$machine{";"}}[0] = 61;

#<(<, =, <=)
@{$machine{"<"}}[0] = 21;
push @{$machine{chr($_)}}, 62 foreach (0..126);
@{$machine{"="}}[21] = 64;
@{$machine{"<"}}[21] = 22;
push @{$machine{chr($_)}}, 63 foreach (0..126);
@{$machine{"="}}[22] = 65;

#=(=)
@{$machine{"="}}[0] = 23;
push @{$machine{chr($_)}}, 66 foreach (0..126);
@{$machine{"="}}[23] = 67;

#>(>, =, >=)
@{$machine{">"}}[0] = 24;
push @{$machine{chr($_)}}, 68 foreach (0..126);
@{$machine{"="}}[24] = 70;
@{$machine{">"}}[24] = 25;
push @{$machine{chr($_)}}, 69 foreach (0..126);
@{$machine{"="}}[25] = 71;

#?
@{$machine{"?"}}[0] = 72;

#@
@{$machine{"@"}}[0] = 84;

#identifier
@{$machine{$_}}[0] = 26 foreach ("A".."Z");
@{$machine{$_}}[0] = 26 foreach ("a".."z");
@{$machine{"_"}}[0] = 26; 
push @{$machine{chr($_)}}, 73 foreach (0..126);
@{$machine{$_}}[26] = 26 foreach ("A".."Z");
@{$machine{$_}}[26] = 26 foreach ("a".."z");
@{$machine{$_}}[26] = 26 foreach ("0".."9");
@{$machine{"_"}}[26] = 26; 

#[
@{$machine{"["}}[0] = 74;

#\
@{$machine{"\\"}}[0] = 84;

#]
@{$machine{"]"}}[0] = 75;

#^(=)
@{$machine{"^"}}[0] = 27;
push @{$machine{chr($_)}}, 76 foreach (0..126);
@{$machine{"="}}[27] = 77;

#`
@{$machine{"`"}}[0] = 84;

#{
@{$machine{"{"}}[0] = 78;

#|(|, =)
@{$machine{"|"}}[0] = 28;
push @{$machine{chr($_)}}, 79 foreach (0..126);
@{$machine{"|"}}[28] = 80;
@{$machine{"="}}[28] = 81;

#}
@{$machine{"}"}}[0] = 82;

#~
@{$machine{"~"}}[0] = 83;

print $fic "#include <vector>\n\nstd::vector<std::vector<int>> machine = {\n";

my @keys = ( 
	"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK",
	"\\a", "\\b", "\\t", "\\n", "\\v", "\\f", "\\r",
	"SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB",
	"ESC", "FS", "GS", "RS", "US"
);

push @keys, chr($_) foreach(32..126);

print $fit "   ";
foreach my $key (0..126) {
	if ($key > 7 && $key <= 16 || $key == 26 || $key >= 29 && $key <= 32) { print $fit " "; }
	if ($key > 32) { print $fit "  "; }
	print $fit "$keys[$key] "
}
print $fit "\n";

my $size = @{$machine{' '}}; #can be any char from 0-126 ASCII
print "$size\n";

foreach my $i (0..$size - 1) {
	print $fit "$i ";
	if ($i < 10) { print $fit " "; }
	print $fic "\t{ ";
	foreach my $key (sort keys %machine) {
		my @a = @{$machine{$key}};
		my $spt = "  ";
		my $spc = ", ";
		if ($a[$i] < 10) { $spt = "   "; $spc = ",  "; }
		if ($a[$i] > 9 && $a[$i] < 100) { $spt = "  "; }
		if ($a[$i] >= 100) { $spt = " "; }
		print $fit "$a[$i]".$spt;
		if (ord($key) == 126) { $spc = " "; }
		print $fic "$a[$i]".$spc;
	}
	print $fit "\n";
	print $fic "}";
	if ($i != $size - 1) { print $fic "," }
	print $fic "\n";
}

print $fic "};";