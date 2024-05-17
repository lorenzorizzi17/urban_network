# Documentation 

Una breve ed esaustica documentazione di quanto realizzato


## Graph creation

Per rendere più realistico il grafo rappresentante il network urbano, sono state implementate due funzioni definite in _headers/randomize.hpp_: _boost::add\_diagonal\_roads(Graph const& g, int N, int SIZE)_ e _boost::remove\_random\_edge(Graph const& g, int N, inst SIZE)_.
- La prima funzione aggiunge randomicamenten $N$ strade diagonali pescando uniformemente un nodo nel grafo e una delle 4 possibili direzioni della strada diagonale che da quel nodo diparte. Un meccanismo interno di protezione impedisce che vengano aggiunti doppi edges (_safety over quantity_: piuttosto non si aggiunge una strada)
- La seconda funzione rimuove casualmente $N$ degli edges nel grafo



## Dijkstra algorithm

Due diverse dinamiche per il modello OD:
- Solo alcuni nodi vengono scelti come origine e solo alcuni vengono selezionati come destinazione
- Ogni nodo ha la stessa probabilità di essere scelto come origine o destinazione 

Optiamo per la seconda
Per modellare la dinamica di agenti microscopici con modello OD sul grafo urbano, ho creato una class user-defined _Agent_. Tale classe ha come membri privati: 
 1) Due oggetti _boost::VertexDescriptor_, rispettivamente la posizione corrente sul grafo e la destinazione
 2) un _m\_internal\_time_, tempo interno della dinamica dell'agente
 3) un _m\_id_, identificativo proprio dell'agente
 4) un _std::vector\<Vertex\> m\_path_ 

 Il costruttore parametrico _Agente(Graph const& g, double MIN\_D)_ istanzia randomicamente il vertice corrente (quindi il vertice di spawn) selezionandone uno fra tutti quelli appartenenti al grafo _g_. Idem per quanto riguarda il vertice di destinazione, scelto anch'esso randomicamente. In teoria, dovrei porre dei vincoli su come viene scelto la destinazione, dovrebbe essere quantomeno leggermente distante dall'origine di spawn. In particolare, il costruttuore sceglierà come destinazione i soli vertici che siano distanti da quello di origine almento _MIN\_D_, dove le distanze sono contate con una chiamata alla funzione _boost::get\_dijkstra\_shortest\_path(Vertex a, Vertex b, Graph g)_ (more on that later). L'identificativo viene aggiornato sfruttando un membro statico che conta le istanze di quella classe. Il costruttore, inoltre, costruisce il vettore _m\_path_ sfruttando la funzione _boost::get\_dijkstra\_shortest\_path(Vertex a, Vertex b, Graph g)_ definita in _headers/dijkstra.hpp_. Tale funzione realizza banalmente la sequenza di vertici da seguire per raggiungere il vertice di destinazione minimizzando il cammino secondo dijkstra. In particolare, ritorna un _std::pair_, il cui primo elemento è il vettore sequena di vertici, il secondo la distanza minima fra p.inizio e fine. Il metodo pubblico _evolve\_dijkstra()_ aggiorna la posizione dell'agente sfruttando tale _m\_path_.

 A questo punto, nel _main.cpp_ viene creato un vettore di tali _Agent_ richiamando il costruttore parametrico randomico e ogni elemento viene fatto evolvere verso la propria destinazione. Quando un agente raggiunge la sua destinazione, viene eliminato dal vettore e il distruttore subentra. Appena ciò accade, un nuovo agente viene istanziato e spawna nel grafo, pronto a seguire la sua strada. __Al momento, la simulazione è settata perché il numero di macchine sul grafo rimanga costante (appena muore, subito nasce)__. Alternative? Ogni tot tempo, ne nasce una




