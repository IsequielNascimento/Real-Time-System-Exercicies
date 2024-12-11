with Ada.Text_IO, Ada.Text_IO, Ada.Calendar;
use Ada.Text_IO, Ada.text_IO, Ada.Calendar; 
procedure test_task is
  
  indice: Integer := 0;
  flagA: Integer:= 1;
  flagB: Integer:= 0;
  flagC: Integer:= 0;
  vetor: array(0..29) of Character;
  x: Integer:= 1;
  i: Integer:= 0;
  
  task type Mostra1 is
    --entry mostra(Item: in Integer);
  end Mostra1;
  task body Mostra1 is
  begin
    while indice < 28 loop
      while flagA = 0 loop
        x:=0;
      end loop;
      vetor (indice):= 'a';
      indice:=indice+1;
      flagA:=0;
      flagB:=1;
    end loop;
end Mostra1;
 
      
  task type Mostra2 is
    --entry mostra(Item: in Integer);
    end Mostra2;
    task body Mostra2 is
    begin
      while indice < 29 loop
        while flagB = 0 loop
           x:=0;
        end loop;
          vetor (indice):= 'b';
          indice:=indice+1;
          flagB:=0;
          flagC:=1;
    end loop;
    end Mostra2;
      
  task type Mostra3 is
    --entry mostra(Item: in Integer);
    end Mostra3;
    task body Mostra3 is
    begin
      while indice < 30 loop
        while flagC = 0 loop
           x:=0;
        end loop;
          vetor (indice):= 'c';
          indice:=indice+1;
          flagA:=1;
          flagC:=0;
    end loop;
    for i in 0..29 loop
      Put(vetor(i));
      end loop;
    end Mostra3;
    
      
    A : Mostra1;
    B : Mostra2;
    C : Mostra3;
    
begin
		i:=0;
end test_task;