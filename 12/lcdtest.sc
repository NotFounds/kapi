byte p4ddr:     0xFEE003;
byte p4dr:      0xFFFFD3;       (* port4 data register (LCD, LED1, LED2) *)

byte tstr:      0xFFFF60;
byte tmdr:      0xFFFF62;
byte tisra:     0xFFFF64;
byte tcr0:      0xFFFF68;
byte tior0:     0xFFFF69;
word gra0:      0xFFFF6C;

byte smr1:      0xFFFFB8;
byte brr1:      0xFFFFB9;
byte scr1:      0xFFFFBA;
byte tdr1:      0xFFFFBB;
byte ssr1:      0xFFFFBC;
byte rdr1:      0xFFFFBD;

byte led_data;  (* RAM for LCD and LED *)
byte s;

function wait_ms(word ms)
byte res;
begin
        while ms <> 0 do
        begin
                tstr := tstr or 0xf9;
                repeat res := (tisra and 0x01) until res = 0x01;
                tstr := tstr and 0xfe;
                tisra := tisra and 0xfe;
                ms := ms - 1;
        end;
end;

function wait_450ns()
byte count;
begin
        count := 100;
        while count <> 0 do count := count - 1;
end;

function lcd_out8(byte data)
byte lcd;
begin
        lcd := (led_data and 0xc0) + (data and 0x3f);
        p4dr := lcd;
	wait_450ns();
	lcd := lcd and 0xdf;
	p4dr := lcd;
	wait_450ns();
end;

function lcd_out4(byte data, byte rs)
byte tmp1;
byte tmp2;
byte lcd;
begin
        tmp1 := (led_data and 0xc0) + rs << 4;

	tmp2 := (data and 0xf0) >> 4;
	lcd := tmp1 + tmp2;
        p4dr := lcd or 0x20;
        wait_450ns();
        lcd := lcd and 0xdf;
        p4dr := lcd;
        wait_450ns();

        tmp2 := data and 0x0f;
        lcd := tmp1 + tmp2;
        p4dr := lcd or 0x20;
        wait_450ns();
        lcd := lcd and 0xdf;
        p4dr := lcd;
        wait_450ns();
end;

function init()
byte count;
begin
        p4ddr := 0xff;  (* set port4 as output *)

        led_data := 0x00;

        tmdr := 0x98;
        tcr0 := 0xa3;
        tior0 := 0x8a;
        gra0 := 2500;

        scr1 := 0;
        smr1 := 0;
        brr1 := 64;
        count := 280;
        while count <> 0 do count := count - 1;
        scr1 := 0x30;
        ssr1 := 0x80;

        wait_ms(15);

        lcd_out8(0x23); (* LCD function set 1 *)
        wait_ms(5);

        lcd_out8(0x23); (* LCD function set 2 *)
        wait_ms(1);

        lcd_out8(0x23); (* LCD function set 3 *)
        wait_ms(1);

        lcd_out8(0x22); (* LCD function set 4 *)
        wait_ms(1);

        lcd_out4(0x28, 0);      (* function set *)
        wait_ms(1);

        lcd_out4(0x0c, 0);      (* display on, cursor off *)
        wait_ms(1);

        lcd_out4(0x06, 0);      (* entry mode set *)
        wait_ms(1);

        lcd_out4(0x01, 0);      (* clear display *)
        wait_ms(2);
end;

(* output single char to LCD *)
function put_char(byte c)
begin
        lcd_out4(c, 1);
        wait_ms(1);
end;

begin
        init();
loop:
        s := ssr1;
        while (s and 0x78) = 0 do s := ssr1;
        if (s and 0x40) <> 0 then
                begin
                if (rdr1 >= 0x20) and (rdr1 <= 0x7E) then
                        begin
                                led_data := led_data or 0x80;
                                p4dr := led_data;

                                put_char(rdr1);
                                ssr1 := s and 0xbf;

                                led_data := led_data and 0x7f;
                                p4dr := led_data;
                        end
                else if rdr1 = 0x0D then
                        begin
                                led_data := led_data or 0x40;
                                p4dr := led_data;

                                lcd_out4(0x01, 0);
                                wait_ms(2);
                                lcd_out4(0x80, 0);
                                wait_ms(1);
                                ssr1 := s and 0xbf;

                                led_data := led_data and 0xbf;
                                p4dr := led_data;
                        end
                else
                        ssr1 := s and 0xbf;
                end
        else
                begin
                        ssr1 := s and 0xc7;
                end;

        goto loop;
end
