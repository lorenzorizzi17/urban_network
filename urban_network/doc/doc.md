# Documentation

Una breve ed esaustiva documentazione di quanto realizzato

## Organizzazione codice
La simulazione è stata implementata su Microsoft Visual Studio Code 2022. La soluzione _urban\_network__ ha un solo progetto dallo stesso nome, la cui compilazione
produce l'eseguibile che lancia la simulazione. Nella directory del progetto, sono state organizzate altre cartelle:
- foc/: contiene la documentazione e la cronistoria del progetto
- fig/: contiene i file .dot e le immagini generate dalla simulazioni (grafi)
- src/: contiene il file di implementazione (uno, _main.cpp_) e, in una cartella separata gli headers file relativi

Il programma utilizza librerie terze per il suo funzionamento. In particolare,
- La Boost Graph Library, per la gestione dei grafi e l'accesso ad algoritmi utili su di essi (ad esempio, Dijkstra). Questa è totalmente headers-only, dunque non richiede linking a librerie esterne
- Libreria SFML per la creazione e manipolazione di finestre grafiche in modo portabile.


## Graph creation

La BGL consente di gestire e manipolare facilmente strutture dati che, per loro natura, si prestano ad una rappresentazione tramite grafi. Attraverso l'estensivo ricorso al generic programming (cite) e ai template, la libreria consente di personalizzare la classe che incapsula il grafo dotandola  di tutte le proprietà che l'utente desidera avere. In particolare, la BGL implementa due classi grafo primitive: _boost::adjacency\_list_ e _boost::adjacency\_matrix_ 
Nel codice in esame, dunque, viene definito un alias _Graph_, risolto come:
_typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,VertexProperty,boost::property<boost::edge_weight_t, double, EdgeProperty>_
La libreria Boost permette di istanziare un grafo direttamente da un file testuale in cui siano presenti, seguendo un certo formato, 
le coppie ordinate di vertici adiacenti. Il file di intestazione _create\_data.hpp_ si occupa esattamente ciò: scrive in un file testuale esterno
(_headers/data/manhattan\_grid.dat_) le combinazioni di vertici collegati in una griglia quadrata stile manhattan (Fig.). 

L'inizializzazione e la gestione della simulazione viene affidata alla classe _Simulation_, che fra i suoi membri privati principali contiene 
un oggetto _boost::Graph_ (alias per ...), grafo orientato planare, e ne viene formattata la
struttura in base al file di testo _headers/data/manhattan\_grid.dat_ appena creato. Il grafo regolare quadrato è pronto (figura)

Per rendere più realistico il grafo rappresentante il network urbano, sono state implementate due funzioni definite in _headers/randomize.hpp_:
_boost::add\_diagonal\_roads(Graph const& g, int N, int SIZE)_ e _boost::remove\_random\_edge(Graph const& g, int N, int SIZE)_.
- La prima funzione aggiunge randomicamente $N$ strade diagonali pescando uniformemente un nodo nel grafo e una delle 4 possibili direzioni della strada diagonale che da quel nodo diparte. Un meccanismo interno di protezione impedisce che vengano aggiunti doppi edges (_safety over quantity_: piuttosto non si aggiunge una strada). Manca ancora un meccanismo che impedisca la creazione di strade che rendano non planare il grafo
- La seconda funzione rimuove casualmente $N$ degli edges nel grafo

Il grafo che rappresenta il reticolo urbano è quindi pronto

## Creazione grafo duale
Nella simulazione che vogliamo costruire, sarebbe piu conveniente associare ad ogni strada un nodo invece che un arco. Due strade sono collegate se 

## Dijkstra algorithm e modello microscopico OD
Partiamo ora con la simulazione della dinamica microscopica/modello OD. Due diverse dinamiche per tale modello:
- Solo alcuni nodi vengono scelti come origine e solo alcuni vengono selezionati come destinazione
- Ogni nodo ha la stessa probabilità di essere scelto come origine o destinazione

Optiamo per la seconda.
Per modellare la dinamica di agenti microscopici con modello OD sul grafo urbano, ho creato una class user-defined _Agent_. Tale classe ha come membri privati:
 1) Due oggetti _boost::EdgeDescriptor_, rispettivamente la posizione corrente sul grafo e la destinazione
 2) un _m\_internal\_time_, tempo interno della dinamica dell'agente
 3) un _m\_id_, identificativo proprio dell'agente
 4) un _std::vector\<Edge\> m\_path_
### Spawn e despawn degli agenti
 Il costruttore parametrico _Agente(Graph const& g, double MIN\_D)_ istanzia randomicamente la strada corrente dell'agente (quindi l'edge di spawn) 
 selezionandone uno fra tutti quelli appartenenti al grafo _g_. Idem per quanto riguarda la strada di destinazione, scelto anch'esso randomicamente. In teoria, dovrei porre dei vincoli su come viene scelto la destinazione, dovrebbe essere quantomeno leggermente distante dall'origine di spawn. In particolare, il costruttore sceglierà come destinazione le sole strade che siano distanti da quello di origine almento _MIN\_D_, dove le distanze sono contate con una chiamata alla funzione _boost::get\_dijkstra\_shortest\_path(Edge a, Edge b, Graph g)_ (more on that later). L'identificativo dell'agente viene impostato sfruttando un membro statico che conta le istanze di quella classe. Il costruttore, inoltre, costruisce il vettore _m\_path_ sfruttando la funzione _boost::get\_dijkstra\_shortest\_path(Edge a, Edge b, Graph g)_ definita in _headers/dijkstra.hpp_.

 Tale funzione realizza banalmente la sequenza di edges da seguire per raggiungere l'edge di destinazione minimizzando il cammino secondo dijkstra. In particolare, ritorna un _std::pair_, il cui primo elemento è il vettore sequenza di strade, il secondo la distanza minima fra edge inizio e fine. Il metodo pubblico _evolve\_dijkstra()_ aggiorna la posizione dell'agente sfruttando tale _m\_path_. Il funzionamento di tale metodo è al momento un poco laborioso e time-consuming ma teoricamente ineccepibile. L'algoritmo di Dijkstra viene chiamato sul grafo duale che viene generato a inizio simulazione: qui, ogni nodo è una strada. Una _std::map\<Edge,Vertex\>_ si occupa di mantenere la connessione fra edge del grafo originale e nodi del duale. Per trasformare i pesi delle strade nel grafo duale, si possono seguire varie strade. Poichè l'algoritmo di Dijkstra per come implementato dalla Boost Library funziona solo con pesi sugli edges, allora associamo ad ogni arco nel grafo duale un peso pari alla somma dei pesi dei nodi (=strade) collegati da tale arco. Tale metodo non è dei più performanti: ogni volta che viene chiamato il costruttore, vengono eseguiti molti algoritmi di dijkstra su di un grafo duale che è decisamente più grande rispetto al grafo originale-fisico. L'alternativa sarebbe quella di girare l'algoritmo di Dijkstra sul grafo diretto, ove però i nodi sono incroci (e le macchine non nascono negli incroci).

 A questo punto, nel _main.cpp_ viene creato un vettore di tali _Agent_ richiamando il costruttore parametrico randomico pronto per essere fatto evolverer. Quando un agente raggiunge la sua destinazione, viene eliminato dal vettore e il distruttore subentra. Appena ciò accade, un nuovo agente viene istanziato e spawna nel grafo, pronto a seguire la sua strada. __Al momento, la simulazione è settata perché il numero di macchine sul grafo rimanga costante (appena muore, subito nasce)__. Alternative? Ogni tot tempo, ne nasce una

 ## Dinamica

La dinamica dovrebbe funzionare così. Inizialmente gli agenti vengono creati sulle strade secondo i metodi già illustrati nel paragrafo precedente.
- Per ogni step temporale $\Delta t$, viene iterato su ogni strada un metodo _boost::flow\_road(Edge const& e)_. Tale metodo permette di far fluire il traffico presente sulla strada $e$-esima attenendosi ai limiti di flusso massimo in uscita hard-coded nella simulazione.

 ## Render grafico
 Il grafo urbano creato tramite istanze di boost è visualizzabile in due modalità:
 - Attraverso il motore di rendering esterno _graphviz_. In _main.cpp_, dopo la chiamate a _boost::create\_manhattan\_data_ e alle funzioni di randomizzazione, il grafo risultante viene salvato in formato .dot in un fike testuale esterno sito in _/data_, col nome _graph.dot_. Attraverso una system call, viene lanciato _graphviz_ che analizza il grafo in tale formato e ne realizza un render grafico salvato in immagine in _graph.png_
 - Attraverso una funzione implementata da me _boost::render\_graph(Graph const& g, sf::RenderWindow const& w, int SIZE, std::vector\<Agent\> const& agents)_ che si appoggia sulla libreria OpenGL SFML. Ad ogni iterazione della simulazione, la chiamata alla funzione _boost::render\_graph(g,w,n,size)_ si preoccupa di disegnare sulla finestra SFML le strade e gli incroci seguendo uno schema più geometrico rispetto al caso precedente, in cui la renderizzazione è interamente affidata a motori di rendering terzi.