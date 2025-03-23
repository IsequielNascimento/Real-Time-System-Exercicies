procedure Concurrent_System is
   Canal : array (0 .. 5) of Integer := (-1, -1, -1, -1, -1, -1);

   procedure Send_Async (Buf : in Integer; C : in Integer) is
   begin
      Canal(C) := Buf;
   end Send_Async;

   procedure Receive (Buf : out Integer; C : in Integer) is
   begin
      while Canal(C) = -1 loop
         null;
      end loop;
      Buf := Canal(C);
      Canal(C) := -1;
   end Receive;

   function Comparacao (Vetor : in array (0 .. 2) of Integer; Versao_Erro : out Integer) return Integer is
   begin
      if (Vetor(0) = Vetor(1)) and then (Vetor(1) = Vetor(2)) then
         Versao_Erro := 0;
         return Vetor(0);
      elsif (Vetor(0) = Vetor(1)) and then (Vetor(1) /= Vetor(2)) then
         Versao_Erro := 3;
         return Vetor(0);
      elsif (Vetor(0) /= Vetor(1)) and then (Vetor(1) = Vetor(2)) then
         Versao_Erro := 1;
         return Vetor(1);
      else
         Versao_Erro := 2;
         return Vetor(0);
      end if;
   end Comparacao;

   task type Thread_A is
   end Thread_A;

   task body Thread_A is
      Voto : Integer := 10;
      Status : Integer;
   begin
      Send_Async(Voto, 0);
      Receive(Status, 3);
      if Status = 0 then
         Put_Line("TA tem continuidade - Prossegue!");
         loop
            null;
         end loop;
      else
         Put_Line("TA gera erro!");
      end if;
   end Thread_A;

   task type Thread_B is
   end Thread_B;

   task body Thread_B is
      Voto : Integer := 8;
      Status : Integer;
   begin
      Send_Async(Voto, 1);
      Receive(Status, 4);
      if Status = 0 then
         Put_Line("TB tem continuidade - Prossegue!");
         loop
            null;
         end loop;
      else
         Put_Line("TB gera erro!");
      end if;
   end Thread_B;

   task type Thread_C is
   end Thread_C;

   task body Thread_C is
      Voto : Integer := 10;
      Status : Integer;
   begin
      Send_Async(Voto, 2);
      Receive(Status, 5);
      if Status = 0 then
         Put_Line("TC tem continuidade - Prossegue!");
         loop
            null; 
         end loop;
      else
         Put_Line("TC gera erro!");
      end if;
   end Thread_C;

   task type Driver is
   end Driver;

   task body Driver is
      Vetor_Comp : array (0 .. 2) of Integer := (others => 0);
      Status_Ok : Integer := 0;
      Status_Erro : Integer := 1;
      Voto_Majoritario : Integer;
      Versao_Erro : Integer;
   begin
      Receive(Vetor_Comp(0), 0);
      Receive(Vetor_Comp(1), 1);
      Receive(Vetor_Comp(2), 2);

      Voto_Majoritario := Comparacao(Vetor_Comp, Versao_Erro);
      Put_Line("Resposta Correta: " & Integer'Image(Voto_Majoritario));

      case Versao_Erro is
         when 0 =>
            Send_Async(Status_Ok, 3);
            Send_Async(Status_Ok, 4);
            Send_Async(Status_Ok, 5);
         when 1 =>
            Send_Async(Status_Erro, 3);
            Send_Async(Status_Ok, 4);
            Send_Async(Status_Ok, 5);
         when 2 =>
            Send_Async(Status_Ok, 3);
            Send_Async(Status_Erro, 4);
            Send_Async(Status_Ok, 5);
         when 3 =>
            Send_Async(Status_Ok, 3);
            Send_Async(Status_Ok, 4);
            Send_Async(Status_Erro, 5);
      end case;
   end Driver;

   A : Thread_A;
   B : Thread_B;
   C : Thread_C;
   D : Driver;
begin
   null; 
end Concurrent_System;
