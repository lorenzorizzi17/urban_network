# Documentation 




## Graph creation




## Dijkstra algorithm

Due diverse dinamiche per il modello OD:
- Solo alcuni nodi vengono scelti come origine e solo alcuni vengono selezionati come destinazione
- Ogni nodo ha la stessa probabilità di essere scelto come origine o destinazione 

Per modellare la dinamica di agenti microscopici con modello OD sul grafo urbano, ho creato una class user-defined _Agent_. Tale classe ha come membri privati: 
 1) Due oggetti _boost::VertexDescriptor_, rispettivamente la posizione corrente sul grafo e la destinazione
 2) un _m\_internal\_time_, tempo interno della dinamica dell'agente
 3) un _m\_id_, identificativo proprio dell'agente
 4) un _std::vector\<Vertex\> m\_path_ 

 Il costruttore parametrico _Agente(Graph const& g)_ istanzia randomicamente il vertice corrente (quindi il vertice di spawn) selezionandone uno fra tutti quelli appartenenti al grafo _g_. Idem per quanto riguarda il vertice di destinazione, scelto anch'esso randomicamente. L'identificativo viene aggiornato sfruttando un membro statico che conta le istanze di quella classe. Il costruttore, inoltre, costruisce il vettore _m\_path_ sfruttando la funzione _boost::get\_dijkstra\_shortest\_path(Vertex a, Vertex b, Graph g)_ definita in _headers/dijkstra.hpp_. Tale funzione realizza banalmente la sequenza di vertici da seguire per raggiungere il vertice di destinazione minimizzando il cammino secondo dijkstra. Il metodo pubblico _evolve\_dijkstra()_ aggiorna la posizione dell'agente sfruttando tale _m\_path_.

 A questo punto, nel _main.cpp_ viene creato un vettore di tali _Agent_ richiamando il costruttore parametrico randomico e ogni elemento viene fatto evolvere verso la propria destinazione. Quando un agente raggiunge la sua destinazione, viene eliminato dal vettore e il distruttore subentra. Appena ciò accade, un nuovo agente viene istanziato e spawna nel grafo, pronto a seguire la sua strada




