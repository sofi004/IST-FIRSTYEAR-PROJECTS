% Sofia Dinis Pinto Piteira Ist1106194
:- set_prolog_flag(answer_write_options,[max_depth(0)]). % para listas completas
:- ['dados.pl'], ['keywords.pl']. % ficheiros a importar.

/*
Codigo
*/

% Pesquisa numa base de dados.

/*
Estrutura dos dados:

evento(ID, NomeDisciplina, Tipologia, NumAlunos, Sala).
turno(ID, SiglaCurso, Ano, NomeTurma).
horario(ID, DiaSemana, HoraInicio, HoraFim,Duracao, Periodo).

Keywords:
salas(TipoSala, ListaSalas).
licenciaturas(tagus,['lee', 'legi', 'leic-t', 'leti']).
mestrados(tagus,['mbmrp', 'mee', 'megi', 'meic-t', 'meti']).
*/


% Qualidade dos dados.

/*
O predicado eventosSemSalas(Eventos) de aridade 1 e 
verdade se Eventos e uma lista, ordenada e sem elementos
repetidos, com os IDs correspondentes aos eventos sem sala. 
*/
eventosSemSalas(Eventos) :- 
    findall(Ev,evento(Ev,_,_,_,semSala),Eventos).

/*
eventosSemSalasDiaSemana(Dia,Event) de aridade 2 e verdade se
Eventos e uma lista, ordenada e sem elementos repetidos, com
os IDs correspondentes aos eventos sem sala que decorrem em Dia.
*/
eventosSemSalasDiaSemana(Dia,Event):- 
    findall(Ev,(horario(Ev,Dia,_,_,_,_),evento(Ev,_,_,_,semSala)),Event).

/*
eventosSemSalasP(PeriodoList,ID) de aridade 2 e verdade se ID e uma lista,
ordenada e sem elementos repetidos, com os IDs dos eventos sem sala, que 
decorrem unicamente nos periodos da lista PeriodoList.
*/
eventosSemSalasP(PeriodoList,ID):-
    findall(Ev,(horario(Ev,_,_,_,_,Periodo),evento(Ev,_,_,_,semSala),
    member(Periodo,PeriodoList)), ID).

/*
eventosSemSalasP_2(PeriodoList, ID) de aridade 2 e verdade se ID e uma lista,
ordenada e sem elementos repetidos, com os IDs dos eventos sem sala, que
decorrem ao longo de todo o primeiro semestre, isto se PeriodoList contem
p1 ou p2.
Caso PeriodoList nao contenha p1 ou p2 entao ID e uma lista vazia.
*/
eventosSemSalasP_2(PeriodoList, ID):-
    % Porque se utiliza o findall, caso a lista de periodos nao contenha 
    % p1 ou p2 o predicado retorna uma lista vazia.
    findall(Ev,(horario(Ev,_,_,_,_,p1_2),evento(Ev,_,_,_,semSala),
    member(Periodo,PeriodoList), member(Periodo, [p1,p2])), ID).

/*
eventosSemSalasP_4(PeriodoList, ID) de aridade 2 e verdade se ID e uma lista,
ordenada e sem elementos repetidos, com os IDs dos eventos sem sala, que
decorrem ao longo de todo o segundo semestre, isto se PeriodoList contem
p3 ou p4.
Caso PeriodoList nao contenha p3 ou p4 entao ID e uma lista vazia.
*/
eventosSemSalasP_4(PeriodoList, ID):-
    % Porque se utiliza o findall, caso a lista de periodos nao contenha 
    % p3 ou p4 o predicado retorna uma lista vazia.
    findall(Ev,(horario(Ev,_,_,_,_,p3_4),evento(Ev,_,_,_,semSala),
    member(Periodo,PeriodoList), member(Periodo, [p3, p4])), ID).

/*
eventosSemSalasPeriodo(PeriodoList,Eventos) de aridade 2 e verdade se 
PeriodoList e uma lista com periodos e Eventos e uma lista,
ordenada e sem elementos repetidos, com os IDs dos eventos sem sala, que 
decorrem nos periodos da lista PeriodoList.
*/    
eventosSemSalasPeriodo(PeriodoList,Eventos):-
    eventosSemSalasP(PeriodoList,Ev1),
    eventosSemSalasP_2(PeriodoList,Ev2),
    eventosSemSalasP_4(PeriodoList,Ev3),
    % Concatena 3 listas resultantes dos predicados anteriores.
    append([Ev1,Ev2,Ev3],Event), 
    % ordena e remove os IDs repetidos.   
    sort(Event, Eventos).

% Pesquisa Simples.

/*
concatena/3 e verdade nas mesmas ocasioes que o predicado append/3.
Este predicado e gerado por um processo recursivo.
*/
concatena([],L,L) :- !. % base de recursao
% junta recursivamente o primeiro elemento de L1 a L2.
concatena([X|L1],L2,[X|L3]):- concatena(L1,L2,L3).

/*
bubble_sort(Xs, Ys) de aridade 2 e verdade se Ys e a lista 
Xs, mas ordenada.
bubble_sort/2 e um predicado de ordenacao por bubblesort.
Este predicado e gerado por um processo recursivo.
*/
bubble_sort(Xs, Ys):-
  concatena(Ws, [A,B|Zs], Xs),
  % compara os numeros de uma lista e se o menor
  % tiver maior indice troca-os de lugar.
  B < A, 
  concatena(Ws, [B,A|Zs], Vs),
  bubble_sort(Vs, Ys), !.
bubble_sort(Xs, Xs).

/*
eventos_periodo(LEv,P,Ev) de aridade 3 e verdade se Ev e uma lista
com todos os eventos cujos IDs estao em LEv e cujos respetivos 
eventos decorrem unicamente no periodo P.
Este predicado e gerado por um processo iterativo.
*/
eventos_periodo(LEv,P,Ev):-eventos_periodo(LEv,P,Ev,[]). % engorda o predicado
eventos_periodo([],_,Acc,Acc).
eventos_periodo([H|T],P,L,Ac):-
    horario(H,_,_,_,_,P),evento(H,_,_,_,_),
    concatena(Ac,[H],Ac1),
    eventos_periodo(T,P,L,Ac1).
eventos_periodo([_|T],P,L,Ac):- eventos_periodo(T,P,L,Ac).

/*
eventos_periodo_2(LEv,P,Ev) de aridade 3 e verdade se Ev e uma lista
com todos os eventos cujos IDs estao em LEv e cujos respetivos
eventos decorrem ao longo de todo o primeiro semestre.
Este predicado e gerado por um processo iterativo.
*/
eventos_periodo_2(LEv,P,Ev):-eventos_periodo_2(LEv,P,Ev,[]). % engorda o predicado
eventos_periodo_2(_,P,[],[]):- P \==p1, P \==p2 .
eventos_periodo_2([],_,Acc,Acc).
eventos_periodo_2([H|T],P,L,Ac):-
    horario(H,_,_,_,_,p1_2),evento(H,_,_,_,_),
    concatena(Ac,[H],Ac1),
    eventos_periodo_2(T,P,L,Ac1).
eventos_periodo_2([_|T],P,L,Ac):- eventos_periodo_2(T,P,L,Ac).

/*
eventos_periodo_4(LEv,P,Ev) de aridade 3 e verdade se Ev e uma lista
com todos os eventos cujos IDs estao em LEv e cujos respetivos eventos
decorrem ao longo de todo o segundo semestre.
Este predicado e gerado por um processo iterativo.
*/
eventos_periodo_4(LEv,P,Ev):-eventos_periodo_4(LEv,P,Ev,[]). % engorda o predicado
eventos_periodo_4(_,P,[],[]):- P \==p3, P\==p4.
eventos_periodo_4([],_,Acc,Acc).
eventos_periodo_4([H|T],P,L,Ac):-
    horario(H,_,_,_,_,p3_4),evento(H,_,_,_,_),
    concatena(Ac,[H],Ac1),
    eventos_periodo_4(T,P,L,Ac1).
eventos_periodo_4([_|T],P,L,Ac):- eventos_periodo_4(T,P,L,Ac).

/*
organizaEventos(ListaEventos, Periodo, EventosNoPeriodo) de aridade 3 e verdade se 
EventosNoPeriodo e uma lista,ordenada e sem elementos repetidos , com todos os eventos
que pertencem a ListaEventos e que decorrem nos periodos contidos na lista Periodo. 
*/
organizaEventos(ListaEventos, Periodo, EventosNoPeriodo) :- 
    eventos_periodo(ListaEventos,Periodo,Ev1),
    eventos_periodo_2(ListaEventos,Periodo,Ev2),
    eventos_periodo_4(ListaEventos,Periodo,Ev3),
    % Concatena 3 listas resultantes dos predicados anteriores.
    concatena(Ev1,Ev2,Ev_1_2), 
    concatena(Ev_1_2,Ev3,Eventos),
    % ordenando os IDs repetidos.
    bubble_sort(Eventos, EventosNoPeriodo).

/*
eventosMenoresQue(Duracao, ListaEventosMenoresQue) de aridade 2 e verdade se
ListaEventosMenoresQue contem os eventos cuja duracao e menor ou igual a Duracao.
*/
eventosMenoresQue(Duracao, ListaEventosMenoresQue):- 
    findall(X,(horario(X,_,_,_,Y,_),Y=<Duracao),ListaEventosMenoresQue).

/*
eventosMenoresQueBool(ID, Duracao) de aridade 2 e verdade se
o evento com o ID especificado tem uma duracao menor ou igual
ao valor fornecido para Duracao. 
*/
eventosMenoresQueBool(ID, Duracao):- 
    horario(ID,_,_,_,D,_), D =< Duracao.

/*
O predicado procuraDisciplinas(Curso, ListaDisciplinas) de aridade 2
e verdade se  ListaDisciplinas e uma lista ordenada com o nome das
disciplinas do curso.
*/
procuraDisciplinas(Curso, ListaDisciplinas):- 
    findall(X,(turno(Ev,Curso,_,_),evento(Ev,X,_,_,_)),D),
    list_to_set(D, Disciplinas), sort(Disciplinas, ListaDisciplinas).

/*
msort(Keys, KeysS) de aridade 2 e verdade se KeysS e a lista
Keys ordenada alfabeticamente.
Este predicado divide a lista de chaves em duas sublistas,
ordena cada uma delas e, em seguida, junta as de volta em 
uma unica lista ordenada.
*/
msort(Keys, KeysS) :-
   keys_pairs(Keys, Pairs),
   keysort(Pairs, PairsS),
   pairs_keys(PairsS, KeysS).

/*
keys_pairs(Ks, Ps) de aridade 2 e verdade se Ps e a transformada de Ks,
sendo Ps uma lista de chaves e Ks uma lista de pares chave-valor,
onde o valor e sempre um elemento vazio (_).
*/
keys_pairs([], []). % base de recursao
keys_pairs([K|Ks], [K-_|Ps]) :-
   keys_pairs(Ks, Ps).

/*
pairs_keys(Ps, Ks) de aridade 2 e verdade se  Ps e a transformada de Ks,
sendo Ps uma lista de chaves e Ks uma lista de pares chave-valor,
onde o valor e sempre um elemento vazio (_).
*/
pairs_keys([], []).
pairs_keys([K-_|Ps],[K|Ks]) :-
   pairs_keys(Ps, Ks).

/*
list_of_lists(L1,L2,L3) de aridade 3 e verdade se L3 e uma lista de
2 listas, sendo a primeira lista de L3 a L1 e a segunda a L2.
*/
list_of_lists(L1,L2,[L1,L2]). 

/*
list_of_lists_quantity([L1,L2], Soma) e verdade se Soma e a soma do numero
de elementos da lista L1 com o numero de elementos da lista L2.
*/
list_of_lists_quantity([L1,L2], Soma):-
    length(L1, Quantity1),
    length(L2, Quantity2),
    Soma is Quantity1 + Quantity2.

/*
remove_duplicates(Repetidos, NaoRepetidos) de aridade 2 e verdade se
a lista NaoRepetidos e a lista Repetidos sem os seus elementos repetidos.
Este predicado e gerado por um processo recursivo.
*/
remove_duplicates([],[]):- !. % base de recursao
remove_duplicates([H | T], List) :-    
     member(H, T),
     remove_duplicates(T, List).
remove_duplicates([H | T], [H|T1]) :- 
      \+member(H, T),
      remove_duplicates(T, T1).

/*
remove_duplicates_listas([L1,L2],[L1,L3]) de aridade 2 e verdade se L3
e a lista L2 sem os elementos pertencentes a L1.
Este predicado e gerado por um processo recursivo.
*/
% base de recursao
remove_duplicates_listas([[],[]],[[],[]]):- !.
remove_duplicates_listas([L1,[]],[L1,[]]):- !.
remove_duplicates_listas([[],L2],[[],L2]):- !.
remove_duplicates_listas([L1,L2],[L1,L3]) :- 
    findall(X,(member(X,L1), member(X,L2)), Members),
    subtract(L2, Members, L3).

/*
O lista_Ev1(ListaDisciplinas, Curso, L) de aridade 3 e verdade se L e uma lista
com as disciplinas de ListaDisciplinas que ocorrem no primeiro semestre e que
fazem parte do plano curricular do Curso.
Este predicado e gerado por um processo iterativo.
*/
lista_Ev1(ListaDisciplinas, Curso, L):- 
    lista_Ev1(ListaDisciplinas, Curso, L, []). % engorda o predicado
    lista_Ev1([],_,Acc,Acc).
lista_Ev1([H|T],Curso,L,Ac):-
    evento(X,H,_,_,_), turno(X,Curso,_,_),
    member(P,[p1,p2,p1_2]),
    horario(X,_,_,_,_,P),
    concatena(Ac,[H],Ac1),
    lista_Ev1(T,Curso,L,Ac1).
lista_Ev1([_|T],Curso,L,Ac):- lista_Ev1(T,Curso,L,Ac).

/*
O lista_Ev2(ListaDisciplinas, Curso, L) de aridade 3 e verdade se L e uma lista
com as disciplinas de ListaDisciplinas que ocorrem no segundo semestre e que
fazem parte do plano curricular do Curso.
Este predicado e gerado por um processo iterativo.
*/
lista_Ev2(ListaDisciplinas, Curso, L):- 
    lista_Ev2(ListaDisciplinas, Curso, L, []). % engorda o predicado
    lista_Ev2([],_,Acc,Acc).
    lista_Ev2([H|T],Curso,L,Ac):-
    evento(X,H,_,_,_), turno(X,Curso,_,_),
    member(P,[p3,p4,p3_4]),
    horario(X,_,_,_,_,P),
    concatena(Ac,[H],Ac1),
    lista_Ev2(T,Curso,L,Ac1).
lista_Ev2([_|T],Curso,L,Ac):- lista_Ev2(T,Curso,L,Ac).

/*
organizaDisciplinas(ListaDisciplinas, Curso, Semestres) de aridade 3 e verdade se  
Semestres e uma lista com duas listas sendo que a primeira contem as disciplinas de
ListaDisciplinas que ocorrem no primeiro semestre e a segunda a que contem as disciplinas
de ListaDisciplinas que ocorrem no segundo semestre. 
Para que a a disciplina esteja em alguma das listas, o Curso tem que ter essa disciplina.
Se alguma das disciplinas em ListaDisciplinas nao for disciplina do Curso o predicado falha.
*/
organizaDisciplinas(ListaDisciplinas, Curso, Semestres) :- 
    lista_Ev1(ListaDisciplinas, Curso, Ev1),
    lista_Ev2(ListaDisciplinas, Curso, Ev2),
    remove_duplicates(ListaDisciplinas,ListaDisciplinasSemRepetidos),
    length(ListaDisciplinasSemRepetidos, Quantidade),
    msort(Ev1,EV1),msort(Ev2,EV2), 
    remove_duplicates(EV1,EV_1),
    remove_duplicates(EV2,EV_2),
    list_of_lists(EV_1,EV_2,S),
    remove_duplicates_listas(S, Semestres),
    list_of_lists_quantity(Semestres, SomaQuantidadeSolucao),
    Quantidade == SomaQuantidadeSolucao. 

/*
eventosCurso_1P(Periodo, Curso, Ano, TotalEventos) de aridade 4 e verdade se 
TotalEventos e uma lista com todos os IDs dos eventos, de disciplinas nao semestrais,
do um Curso que ocorrem no Ano e Periodo.
*/
eventosCurso_1P(Periodo, Curso, Ano, TotalEventos):-
    findall(Ev,(turno(Ev,Curso,Ano,_), horario(Ev,_,_,_,_,Periodo)),TotalEventos).

/*
eventosCurso_2(Periodo, Curso, Ano, TotalEventos) de aridade 4 e verdade se
TotalEventos e uma lista com todos os IDs dos eventos de um Curso que ocorrem
no primeiro semestre do Ano.
Se o Periodo nao for p1 ou p2, para que o predicado seja verdade, TotalEventos
tem que ser uma lista vazia.
*/
eventosCurso_2(P, _, _,[]):- P ==p4.
eventosCurso_2(P, _, _,[]):- P ==p3.
eventosCurso_2(_, Curso, Ano, TotalEventos):-
    findall(Ev,(turno(Ev,Curso,Ano,_), horario(Ev,_,_,_,_,p1_2)),TotalEventos).

/*
eventosCurso_2(Periodo, Curso, Ano, TotalEventos) de aridade 4 e verdade se
TotalEventos e uma lista com todos os IDs dos eventos de um Curso que ocorrem
no segundo semestre do Ano.
Se o Periodo nao for p3 ou p4, para que o predicado seja verdade, TotalEventos
tem que ser uma lista vazia.
*/
eventosCurso_4(P, _, _,[]):- P == p1.
eventosCurso_4(P, _, _,[]):- P == p2.
eventosCurso_4(_, Curso, Ano, TotalEventos):-
    findall(Ev,(turno(Ev,Curso,Ano,_), horario(Ev,_,_,_,_,p3_4)),TotalEventos).

/*
eventosCurso(Periodo, Curso, Ano, TotalEventos) de aridade 4
e verdade se TotalEventos e uma lista com todos os IDs dos
eventos do Curso que ocorrem no Ano e Periodo.
*/
eventosCurso(Periodo, Curso, Ano, TotalEventos):-
    eventosCurso_1P(Periodo, Curso, Ano, Eventos_1P),
    eventosCurso_2(Periodo, Curso, Ano, Eventos_2),
    eventosCurso_4(Periodo, Curso, Ano, Eventos_4),
    append(Eventos_1P, Eventos_2, List_of_events),
    append(List_of_events, Eventos_4, List_of_events_final),
remove_duplicates(List_of_events_final, TotalEventos).

/*
O predicado horasCurso(Periodo, Curso, Ano, TotalHoras) de aridade 4 
e verdade se TotalHoras e a soma das duracoes dos eventos, do Curso,
que ocorrem no Ano e Periodo.
*/
horasCurso(Periodo, Curso, Ano, TotalHoras):-
    eventosCurso(Periodo, Curso, Ano, TotalEventos),
    findall(X,(horario(Ev,_,_,_,X,_), member(Ev, TotalEventos)), List_of_hours),
sum_list(List_of_hours, TotalHoras).

/*
O predicado horas_curso_tuplo(Periodo, Curso, Ano, Tuplo) de aridade 4 
e verdade se Tuplo e um tuplo com informacao sobre
o numero de horas de um curso num determinado periodo e ano. 
O tuplo criado e da seguinte forma (Ano, Periodo, TotalHoras).
Sendo TotalHoras numero de horas de um curso num determinado periodo e ano.
*/
horas_curso_tuplo(Periodo, Curso, Ano, (Ano, Periodo, TotalHoras)):-
    horasCurso(Periodo, Curso, Ano, TotalHoras).

/*
evolucaoHorasCurso(Curso, ListaTuplos) de aridade 2 e verdade se ListaTuplos
e uma lista, ordenada por ano e periodo crescente, de tuplos da forma 
(Ano, Periodo, NumHoras), em que NumHoras e o total de horas
associadas ao curso, num Ano e num Periodo.
*/
evolucaoHorasCurso(Curso, [P1_1, P2_1, P3_1, P4_1, P1_2, P2_2, P3_2, P4_2, P1_3,P2_3, P3_3, P4_3]):-
    horas_curso_tuplo(p1, Curso, 1, P1_1),
    horas_curso_tuplo(p2, Curso, 1, P2_1),
    horas_curso_tuplo(p3, Curso, 1, P3_1),
    horas_curso_tuplo(p4, Curso, 1, P4_1),
    horas_curso_tuplo(p1, Curso, 2, P1_2),
    horas_curso_tuplo(p2, Curso, 2, P2_2),
    horas_curso_tuplo(p3, Curso, 2, P3_2),
    horas_curso_tuplo(p4, Curso, 2, P4_2),
    horas_curso_tuplo(p1, Curso, 3, P1_3),
    horas_curso_tuplo(p2, Curso, 3, P2_3),
    horas_curso_tuplo(p3, Curso, 3, P3_3),
    horas_curso_tuplo(p4, Curso, 3, P4_3).

% Ocupacoes criticas das salas 

/*
ocupaSlot/5 verifica se um evento ocorre durante um periodo de 
tempo especificado, ou seja, se o evento ocupa o slot de tempo dado.
Se o evento ocorre durante o periodo de tempo especificado, o predicado
atribui a variavel Horas o tempo ocupado pelo evento no periodo especificado.
Se o evento nao ocorre durante o periodo de tempo especificado, o predicado falha.
*/

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas):-
    HoraInicioDada =< HoraInicioEvento,
    HoraInicioEvento < HoraFimEvento, 
    HoraFimEvento =< HoraFimDada, !,
    Horas is -(HoraFimEvento,HoraInicioEvento).

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas):-
    HoraInicioEvento =< HoraInicioDada,
    HoraInicioDada < HoraFimDada, 
    HoraFimDada =< HoraFimEvento, !,
    Horas is -(HoraFimDada,HoraInicioDada).

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas):-
    HoraInicioDada =< HoraInicioEvento,
    HoraInicioEvento < HoraFimDada, 
    HoraFimDada =< HoraFimEvento, !,
    Horas is -(HoraFimDada,HoraInicioEvento).

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas):-
    HoraInicioEvento =< HoraInicioDada,
    HoraInicioDada < HoraFimEvento, 
    HoraFimEvento =< HoraFimDada, !,
    Horas is -(HoraFimEvento,HoraInicioDada).

 
ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, _):-
    % 3 condicoes que em conjunto levam o predicado a falhar.
    HoraInicioDada < HoraFimDada,
    HoraFimDada < HoraInicioEvento,
    HoraInicioEvento < HoraFimEvento, !,
    % falhanco forcado.
    fail.

ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, _):-
    % 3 condicoes que em conjunto levam o predicado a falhar.
    HoraInicioEvento < HoraFimEvento,
    HoraFimEvento < HoraInicioDada,
    HoraInicioDada < HoraFimDada, !,
    % falhanco forcado.
    fail. 

/*
O predicado numHoras_1P(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours) de aridade 6
e verdade se List_of_hours e uma lista com resultados obtidos atraves do predicado ocupaSlot/5, quando
inserimos os 4 primeiros argumentos do predicado.
Os 2 primeiros argumentos do predicado ocupaSlot/5 sao dados como argumentos de entrada e os dois 
seguintes sao a hora inicial e final dos eventos, de disciplinas nao semestrais, que ocorrem no Periodo,
DiaSemana e TipoSala.
*/
numHoras_1P(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours):-
    findall(Horas,(salas(TipoSala, Lista_salas), evento(Ev,_,_,_,Sala), horario(Ev,DiaSemana,Hi,Hf,_,Periodo), 
    member(Sala,Lista_salas), ocupaSlot(HoraInicio, HoraFim, Hi, Hf, Horas)), List_of_hours).

/*
numHoras_2(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours) de aridade 6
e verdade se List_of_hours e uma lista com resultados obtidos atraves do predicado ocupaSlot/5,
quando inserimos os 4 primeiros argumentos do predicado.
Os 2 primeiros argumentos do predicado ocupaSlot/5 sao dados como argumentos de entrada
e os dois seguintes sao a hora inicial e final dos eventos, de disciplinas que ocorrem 
ao longo de todo o primeiro semestre, no DiaSemana e TipoSala.
Se o Periodo for p3 ou p4 para que o predicado seja verdade List_of_hours tem que ser uma lista vazia.
*/
numHoras_2(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours):-
    findall(Horas,(salas(TipoSala, Lista_salas), member(Periodo, [p1, p2]),
    member(Sala, Lista_salas), evento(Ev,_,_,_,Sala), horario(Ev,DiaSemana,Hi,Hf,_,p1_2),
    ocupaSlot(HoraInicio, HoraFim, Hi, Hf, Horas)), List_of_hours).

/*
numHoras_2(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours) de aridade 6
e verdade se List_of_hours e uma lista com resultados obtidos atraves do predicado ocupaSlot/5,
quando inserimos os 4 primeiros argumentos do predicado.
Os 2 primeiros argumentos do predicado ocupaSlot/5 sao dados como argumentos de entrada
e os dois seguintes sao a hora inicial e final dos eventos, de disciplinas que ocorrem 
ao longo de todo o segundo semestre, no DiaSemana e TipoSala.
Se o Periodo for p1 ou p2 para que o predicado seja verdade List_of_hours tem que ser uma lista vazia.
*/
numHoras_4(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours):-
    findall(Horas,(salas(TipoSala, Lista_salas), member(Periodo, [p3, p4]),
    evento(Ev,_,_,_,Sala), horario(Ev,DiaSemana,Hi,Hf,_,p3_4), member(Sala, Lista_salas),
    ocupaSlot(HoraInicio, HoraFim, Hi, Hf, Horas)), List_of_hours).

/*
numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras) de aridade 6 
e verdade se SomaHoras e o numero de horas ocupadas nas salas do TipoSala, no intervalo 
entre HoraInicio e HoraFim, no DiaSemana e no Periodo.
*/
numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras):-
    numHoras_1P(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours_1P),
    numHoras_2(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours_2),
    numHoras_4(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, List_of_hours_4),
    % concatenacao das 3 lista provenientes dos 3 predicados anteriormente implementados.
    append(List_of_hours_1P, List_of_hours_2, List_of_hoursAux),
    append(List_of_hoursAux, List_of_hours_4, List_of_hours),
% Soma dos elementos da lista que resultou das concatenacoes anteriores.
sum_list(List_of_hours, SomaHoras).

/*
ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max) de aridade 4
e verdade se Max e o numero maximo de horas que podem ser ocupadas
por um TipoSala entre HoraInicio e HoraFim.
*/
ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max):-
    salas(TipoSala, Lista_salas), 
    % Determina quantas salas de um especificado tipo existem.
    length(Lista_salas, Numero_salas),
    Max is ((HoraFim - HoraInicio) * Numero_salas).

/*
percentagem(SomaHoras, Max, Percentagem) de aridade 3
e verdade se Percentagem e a percentagem de SomaHoras
em relacao a Max.
*/
percentagem(SomaHoras, Max, Percentagem):-
    Percentagem is (SomaHoras/Max)*100.

/*
arredondar(NaoArredondado, Arredondado) de aridade 2
e verdade se Arredondado for o valor NaoArredondado 
arredondado para o inteiro imediatamente superior. 
*/
arredondar(NaoArredondado, Arredondado):- Arredondado is ceiling(NaoArredondado).

/*
O predicado ocupacaoCritica(HoraInicio, HoraFim, Threshold, Resultados) de aridade 4 e verdade se Resultados 
for uma lista ordenada de tuplos do tipo casosCriticos(DiaSemana, TipoSala, Percentagem) em que DiaSemana, 
TipoSala e Percentagem sao, respectivamente, um dia da semana, um tipo de sala e a sua percentagem de ocupacao,
no intervalo de tempo entre HoraInicio e HoraFim em que a ocupacao de uma sala em um determinado periodo de
tempo excede um limite especificado por Threshold.
*/
ocupacaoCritica(HoraInicio, HoraFim, Threshold, Resultados):-
    findall(casosCriticos(DiaSemana, TipoSala, Percentagem), 
    (salas(TipoSala, Salas),
    member(S,Salas), evento(Ev,_,_,_,S), member(Periodo, [p1,p2,p3,p4]),
    horario(Ev,DiaSemana,_,_,_,Periodo),
    numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras),
    ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max),
    percentagem(SomaHoras, Max, Percent), arredondar(Percent, Percentagem),
    Percentagem > Threshold), Res), 
    % Remove os elementos repetidos da lita e ordena os restantes.
sort(Res, Resultados). 

% Verdade se NomePessoa e a pessoa que fica na cabeceira 1 (X4).
restricao(cab1(NomePessoa), ListaPessoas) :- nth1(4, ListaPessoas, NomePessoa).

% Verdade se NomePessoa e a pessoa que fica na cabeceira 2 (X5).
restricao(cab2(NomePessoa), ListaPessoas) :- nth1(5, ListaPessoas, NomePessoa).

% Verdade se NomePessoa1 e NomePessoa2 ficarem lado a lado na mesa.
restricao(lado(NomePessoa1, NomePessoa2), ListaPessoas) :- 
    nth1(Index1, ListaPessoas, NomePessoa1), nth1(Index2, ListaPessoas, NomePessoa2),
    (1 =:= -(Index1, Index2); -1 =:= -(Index1, Index2)),
    Index1 =\= 5, Index1 =\= 4, Index2 =\= 5, Index2 =\= 4.

% Verdade se NomePessoa1 estiver numa das cabeceiras e NomePessoa2 ficar a sua direita.
restricao(honra(NomePessoa1, NomePessoa2), ListaPessoas) :-
    nth1(Index1, ListaPessoas, NomePessoa1), nth1(Index2, ListaPessoas, NomePessoa2),
    (nth1(4, ListaPessoas, NomePessoa1), 2 =:= -(Index2,Index1);
    nth1(5, ListaPessoas, NomePessoa1), 2 =:= -(Index1,Index2)).

% Verdade se NomePessoa1 e NomePessoa2 nao ficarem lado a lado na mesa.
restricao(naoLado(NomePessoa1, NomePessoa2), ListaPessoas) :-
    not(restricao(lado(NomePessoa1, NomePessoa2), ListaPessoas)).

% Ter em atencao que nao se considera cabeceiras frente a frente.

% Verdade se NomePessoa1 e NomePessoa2 ficarem exatamente frente a frente na mesa.
restricao(frente(NomePessoa1, NomePessoa2), ListaPessoas) :-
    nth1(Index1, ListaPessoas, NomePessoa1), nth1(Index2, ListaPessoas, NomePessoa2),
    (5 =:= -(Index1, Index2); -5 =:= -(Index1, Index2)), 
    Index1 =\= 5, Index1 =\= 4, Index2 =\= 5, Index2 =\= 4.

% Verdade se NomePessoa1 e NomePessoa2 nao ficarem frente a frente na mesa.
restricao(naoFrente(NomePessoa1, NomePessoa2), ListaPessoas) :-
    not(restricao(frente(NomePessoa1, NomePessoa2), ListaPessoas)).

/*
restricoes(ListaRestricoes, LP) de aridade 2 e verdade se LP cumpre toda a restricao em ListaRestricoes.
*/
restricoes([], _). % Base de recursao
restricoes([Restricao | RestoRestricoes], LP) :-
    restricao(Restricao, LP),
    restricoes(RestoRestricoes, LP).

/*
ocupacaoMesa(ListaPessoas, ListaRestricoes, OcupacaoMesa) de aridade 3 e verdade se
ListaPessoas for a lista com o nome das pessoas a sentar a mesa, ListaRestricoes
for a lista de restricoes a verificar e OcupacaoMesa for uma lista com tres listas,
em que a primeira contem as pessoas de um lado da mesa (X1, X2 e X3), a segunda as
pessoas a cabeceira (X4 e X5) e a terceira as pessoas do outro lado da mesa (X6, X7 e X8),
de modo a que essas pessoas sao exactamente as da ListaPessoas e verificam todas as
restricoes de ListaRestricoes.
*/
ocupacaoMesa(ListaPessoas, ListaRestricoes, OcupacaoMesa) :-
    findall([X1, X2, X3, X4, X5, X6, X7, X8], 
    (permutation(ListaPessoas, [X1, X2, X3, X4, X5, X6, X7, X8]),
    restricoes(ListaRestricoes, [X1, X2, X3, X4, X5, X6, X7, X8])), Lista),
    % Apenas uma das permutacoes cumpre toda a restricao existente.
    member([X1, X2, X3, X4, X5, X6, X7, X8], Lista),
    OcupacaoMesa = [[X1, X2, X3], [X4, X5], [X6, X7, X8]].