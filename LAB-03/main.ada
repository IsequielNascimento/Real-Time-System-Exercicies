With Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;
Use Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;

procedure code1 is

   protected type Resource is
      entry Seize;
      procedure Release;
   private
      Busy : Boolean := False;
   end Resource;
   protected body Resource is
      entry Seize when not Busy is
      begin
         Busy := True;
      end Seize;
      procedure Release is
      begin
         Busy := False;
      end Release;
   end Resource;

   canal_mens: array(1..8) of Integer;
   canal_rec:  array(1..8) of Resource;
   canal_send: array(1..8) of Resource;

   function send_sync(mensagem: in Integer; canal : in Integer) return Integer is
   begin
      canal_mens(canal):= mensagem;
      canal_rec(canal).release;
      canal_send(canal).seize;
      return 0;
   end send_sync;

   function receive(mensagem: out Integer; canal : in Integer) return Integer is
   begin
      canal_rec(canal).seize;
      mensagem:=canal_mens(canal);
      canal_send(canal).release;
      return 0;
   end receive;

   function converteValor(byte2: in Integer; byte1: in Integer) return Integer is
   begin
      return byte2 * 256 + byte1;
   end converteValor;

   function encontraBytes(valor: in Integer; byte2: out Integer; byte1: out Integer) return Integer is
   begin
      byte1 := valor mod 256;
      byte2 := (valor / 256) mod 256;
      return 0;
   end encontraBytes;


   task type TA is
   end TA;
   task body TA is
      pacote:integer;
      valor:integer := 10;
      x:integer;
   begin
      delay 1.0;
      while True loop
         delay 1.0;
         pacote := converteValor(5, valor);
         x := send_sync(pacote, 1);
      end loop;
   end TA;

   task type TB is
   end TB;
   task body TB is
      pacote:integer;
      valor:integer := 20;
      x:integer;
   begin
      delay 1.0;
      while True loop
         delay 1.0;
        pacote := converteValor(6, valor);
        x := send_sync(pacote, 2);
      end loop;
   end TB;

   task type TC is
   end TC;
   task body TC is
      pacote:integer;
      valor:integer := 30;
      x:integer;
   begin
      delay 1.0;
      while True loop
        delay 1.0;
        pacote := converteValor(7, valor);
        x := send_sync(pacote, 3);
      end loop;
   end TC;

   task type TD is
   end TD;
   task body TD is
      pacote:integer;
      valor:integer := 40;
      x:integer;
   begin
      delay 1.0;
      while True loop
        delay 1.0;
        pacote := converteValor(8, valor);
        x := send_sync(pacote, 4);
      end loop;
   end TD;

   task type TE is
   end TE;
   task body TE is
      pacote:integer;
      x:integer;
   begin
      delay 1.0;
      while True loop
         delay 1.0;
         x := receive(pacote, 5);
         Put_Line("Thread E recebe valor: " & Integer'Image(pacote));
      end loop;
   end TE;

   task type TF is
   end TF;
   task body TF is
      pacote:integer;
      x:integer;
   begin
      delay 1.0;
      while True loop
         delay 1.0;
         x := receive(pacote, 6);
         Put_Line("Thread F recebe valor: " & Integer'Image(pacote));
      end loop;
   end TF;

   task type TG is
   end TG;
   task body TG is
      pacote:integer;
      x:integer;
   begin
      delay 1.0;
      while True loop
         delay 1.0;
         x := receive(pacote, 7);
         Put_Line("Thread G recebe valor: " & Integer'Image(pacote));
      end loop;
   end TG;

   task type TH is
   end TH;
   task body TH is
      pacote:integer;
      x:integer;
   begin
      delay 1.0;
      while True loop
         delay 1.0;
         x := receive(pacote, 8);
         Put_Line("Thread H recebe valor: " & Integer'Image(pacote));
      end loop;
   end TH;

   task type Mailbox is
   end Mailbox;
   task body Mailbox is
      pos:integer;
      destino:integer;
      valor:integer;
      x:integer;
      vetor: array(1..12) of Integer;
   begin
      delay 1.0;
      while True loop
         pos := 1;
         for i in 1..3 loop
            for j in 1..4 loop
               x := receive(vetor(pos), j);
               pos := pos + 1;
            end loop;
         end loop;

         for i in 1..12 loop
            x := encontraBytes(vetor(i), destino, valor);
            x := send_sync(valor, destino);
         end loop;
      end loop;

   end Mailbox;

   A : TA;
   B : TB;
   C : TC;
   D : TD;
   E : TE;
   F : TF;
   G : TG;
   H : TH;
   I : Mailbox;

begin
   for i in 1..8 loop
      canal_send(i).Seize;
      canal_rec(i).Seize;
   end loop;
end code1;
