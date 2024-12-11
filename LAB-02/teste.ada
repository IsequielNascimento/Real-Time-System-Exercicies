with Ada.Text_IO; use Ada.Text_IO;
with Ada.Calendar; use Ada.Calendar;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure N_Version_Program is

   type Buffer_Type is array (1 .. 3) of Integer;
   Canal : Buffer_Type := (others => -1);
   Status : Buffer_Type := (others => 0);
   Voto_Majoritario : Integer := -1;
   Versao_Erro : Integer := -1;

   protected Canal_Protected is
      procedure Send_Async(Index : Integer; Value : Integer);
      procedure Receive(Index : Integer; Value : out Integer);
   private
      Buffer : Buffer_Type := (others => -1);
   end Canal_Protected;

   protected body Canal_Protected is
      procedure Send_Async(Index : Integer; Value : Integer) is
      begin
         Buffer(Index) := Value;
      end Send_Async;

      procedure Receive(Index : Integer; Value : out Integer) is
      begin
         while Buffer(Index) = -1 loop
            null;
         end loop;
         Value := Buffer(Index);
         Buffer(Index) := -1;
      end Receive;
   end Canal_Protected;

   task type Versao (Index : Integer) is
   end Versao;

   task body Versao is
      Voto : Integer;
      Status_Recebido : Integer;
   begin
      if Index = 1 then
         Voto := 5 + 5;
      elsif Index = 2 then
         Voto := 2 * 5;
      elsif Index = 3 then
         Voto := 3 + 7;
      end if;

      Canal_Protected.Send_Async(Index, Voto);
      Canal_Protected.Receive(Index, Status_Recebido);

      if Status_Recebido = 1 then
         Put_Line("Versao " & Integer'Image(Index) & ": Voto incorreto, finalizando.");
      else
         Put_Line("Versao " & Integer'Image(Index) & ": Voto correto, continuando execucao.");
      end if;
   end Versao;

   task Driver is
   end Driver;

   task body Driver is
      Vetor_Comp : Buffer_Type;
   begin
      for I in 1 .. 3 loop
         Canal_Protected.Receive(I, Vetor_Comp(I));
      end loop;

      declare
         Votos : array (1 .. 3) of Integer := (others => 0);
      begin
         for I in 1 .. 3 loop
            Votos(Vetor_Comp(I)) := Votos(Vetor_Comp(I)) + 1;
         end loop;

         for I in 1 .. 3 loop
            if Votos(Vetor_Comp(I)) > 1 then
               Voto_Majoritario := Vetor_Comp(I);
               exit;
            end if;
         end loop;

         for I in 1 .. 3 loop
            if Vetor_Comp(I) /= Voto_Majoritario then
               Versao_Erro := I;
            end if;
         end loop;
      end;

      for I in 1 .. 3 loop
         if I = Versao_Erro then
            Canal_Protected.Send_Async(I, 1);
         else
            Canal_Protected.Send_Async(I, 0);
         end if;
      end loop;

      Put_Line("Voto majoritario: " & Integer'Image(Voto_Majoritario));
      if Versao_Erro /= -1 then
         Put_Line("Versao com voto minoritario: Versao " & Integer'Image(Versao_Erro));
      end if;
   end Driver;

   V1, V2, V3 : Versao (Index => 1);
   D : Driver;

begin
   null;
end N_Version_Program;
