(* SCI channel 1 *)
byte smr1: 0xfffb8;
byte brr1: 0xfffb9;
byte scr1: 0xfffba;
byte tdr1: 0xfffbb;
byte ssr1: 0xfffbc;
byte rdr1: 0xfffbd;
byte c;
byte buffer[256];
long ix;

function init()
word count;
byte c;
byte s;
begin
	scr1 := 0;	(* init *)
	smr1 := 0;	(* async, 8bits, np, stop1 *)
	brr1 := 51;	(* 9600bps *)
	count := 280;
	while count <> 0 do count := count - 1;
	scr1 := 0x30;
	c := ssr1;
	ssr1 := 0x80;
end;

function send(byte c)
byte s;
begin
	s := ssr1;
	while (s and 0x80) = 0 do s:= ssr1;
	tdr1 := c;
	ssr1 := s and 0x7F;
end;

function receive()
byte s;
byte c;
begin
	s := ssr1;
	while (s and 0x78) = 0 do s:= ssr1;
	if (s and 0x40) <> 0 then (* read ok *)
		begin
			c := rdr1;
			ssr1 := s and 0xbf;
		end
	else (* read error *)
		begin
			c := '$';
			ssr1 := s and 0xc7;
		end;
	return c;
end;

begin
	init();
loop:
	send('*');
	ix := 0;
	c := receive(); send(c); (* read and echo back *)
	while ((ix < 256) and (c <> 0x0D)) do
		begin
			buffer[ix] := c;
			ix := ix + 1;
			c := receive(); send(c);  (* read and echo back *)
		end;
	buffer[ix] := '\0'; (* place an end mark *)

	send(0x0A); (* line feed *)
	ix := 0;
	while (buffer[ix] <> '\0') do
		begin
			send(buffer[ix]);
			ix := ix + 1;
		end;
	send(0x0D); (* carriage return *)
	send(0x0A); (* line feed *)

	goto loop;
end
