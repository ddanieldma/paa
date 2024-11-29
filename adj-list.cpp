typedef int vertex;

class EdgeNode {
public:
    EdgeNode(vertex otherVertex, EdgeNode * next)
    : m_otherVertex(otherVertex)
    , m_next(next) {}
    vertex otherVertex() {
        return m_otherVertex;
    }
    EdgeNode * next() {
        return m_next;
    }
    void setNext(EdgeNode * next) {
        m_next = next;
    }
private:
    int m_otherVertex;
    EdgeNode * m_next;
};


class GraphAdjList {
public:
    GraphAdjList(int numVertices);
    ~GraphAdjList();
    void addEdge(vertex v1, vertex v2);
    void removeEdge(vertex v1, vertex v2);
    void print();
private:
    int m_numVertices;
    int m_numEdges;
    EdgeNode ** m_edges;

    GraphAdjList(int numVertices)
        : m_numVertices(numVertices)
        , m_numEdges(0)
        , m_edges(nullptr) {
            m_edges = new EdgeNode*[numVertices];
            for (vertex i=0; i < numVertices; i++) {
            m_edges[i] = nullptr;
        }
    }

    ~GraphAdjList() {
        for (vertex i=0; i < m_numVertices; i++) {
            EdgeNode * edge = m_edges[i];
            while (edge) {
                EdgeNode * next = edge->next();
                delete edge;
                edge = next;
            }
        }
        delete[] m_edges;
    }

    void addEdge(vertex v1, vertex v2) {
        EdgeNode * edge = m_edges[v1];
        while (edge) {
            if (edge->otherVertex() == v2) { // Check if exists
                return;
            }
            edge = edge->next();
        }
        m_edges[v1] = new EdgeNode(v2, m_edges[v1]); // Create the new edge
        m_numEdges++;
    }

    void removeEdge(vertex v1, vertex v2) {
        EdgeNode * edge = m_edges[v1];
        EdgeNode * previousEdge = nullptr;
        while (edge) {
            if (edge->otherVertex() == v2) { // Found!
                if (previousEdge) {
                    previousEdge->setNext(edge->next());
                } else {
                    m_edges[v1] = edge->next();
                }
                delete edge;
                
                break;
            }
            previousEdge = edge;
            edge = edge->next();
        }
    }

    bool hasTopologicalOrder(int order[]) {
        int inDegree[m_numVertices];

        for (vertex i = 0; i < m_numVertices; i++) {
            inDegree[i] = 0;
        }

        for (vertex i = 0; i < m_numVertices; i++) {
            for (EdgeNode * e = m_edges[i]; e != nullptr; e = e->next()) {
                inDegree[e->otherVertex()]++;
            }
        }

        vertex queue[m_numVertices];
        int start = 0;
        int end = 0;

        for (vertex i = 0; i < m_numVertices; i++) {
            if (inDegree[i] == 0) {
                    queue[end++] = i;
            }
        }

        int counter = 0;
        while (start < end) {
            vertex v = queue[start++];
            
            order[v] = counter++;

            for (EdgeNode * e = m_edges[v]; e != nullptr; e = e->next()) {
                vertex other = e->otherVertex();
                inDegree[other]--;
                if (inDegree[other] == 0) {
                    queue[end++] = other;
                }
            }
        }
        return counter >= m_numVertices;
    }

    void cptDijkstraFast(vertex v0, vertex * parent, int * distance){
        // Vetor que identifica se um vértice foi verificado
        bool checked[m_numVertices];
        
        heap Heap; // Criando heap

        // Inicializando vértices
        for (vertex=0; v < m_numVertices; v++){
            parent[v] = -1;
            distance[v] = INT_MAX;
            checked[v] = false;
        }

        // Setando raiz
        parent[v0] = v0;
        distance[v0] = 0;

        // Colocando raiz no heap
        heap.insert_or_update(distance[v0], v0);
        while(!heap.empty()) {
            // Pegando e retirando menor elemento da fila de prioridade
            vertex v1 = heap.top().second;
            heap.pop();

            // Parando execução do algoritmo se encontrarmos um nó inalcançável a partir da origem v0
            if(distance[v1] == INT_MAX) { break; }
            
            // Percorrendo todas as arestas do vértice em questão
            EdgeNode * edge = m_edges[v1];
            while(edge) {
                vertex v2 = edge->otherVertex();
                
                // Se ainda não tivermos checado um vértice na franja
                if (!checked[v2]){
                    // Pegando custo dessa aresta
                    int cost = edge->cost(); //Essa implementação pra WDG
                    
                    // Se a distância por esse caminho for menor que a 
                    // distância que já temos para o vértice...
                    if (distance[v1] + cost < distance[v2]) {
                        // ... o pai dele se torna a vértice em que estamos
                        // (na lista de adj.)...
                        parent[v2] = v1;
                        // ...a sua distância é atualizada...
                        distance[v2] = distance[v1] + cost;
                        // ...e ele é inserido na fila de prioridades.
                        heap.insert_or_update(distance[v2], v2);

                    }
                }

                edge = edge->next;
            }
        }
        checked[v1] = true;
    }

    void initialize(vertex * parent, bool * inTree, int * vertexCost) const{
        // Inicializando tudo
        for (vertex v0 = 0; v < m_numVertices; v++){
            parent[v] = -1;
            inTree[v] = false;
            vertexCost[v] = INT_MAX;
        }

        // Inicializando raiz
        // Definindo-a como sem pai
        parent[0] = 0;
        // Colocando-a na árvore
        inTree[0] = true;

        // Percorrendo todas as arestas da raiz
        EdgeNode * edge = m_edges[0];
        while (edge){
            vertex v2 = edge->otherVertex();

            // Definindo filhos da raiz
            parent[v2] = 0;
            // Colocando custo dos vertices filhos da raiz
            vertexCost[v2] = edge->cost();
            
            edge = edge->next();
        }
        
    }
    
    void mstPrimFastV2(vertex * parent) {
        // Inicialização
        bool inTree[m_numVertices];
        int vertexCost[m_numVertices];
        initialize(parent, inTree, vertexCost);

        // Criação e inicialização do heap
        Heap heap; 
        for (vertex v = 1; v < m_numVertices; v++) { heap.insert_or_update(vertexCost[v], v); }

        while (!heap.empty()) {
            // Pegando e retirando do heap vértice com menor custo
            vertex v1 = heap.top().second;
            heap.pop(); 

            // Se o vértice no heap tiver custo infinito ele é inalcançá
            // vel e encerramos o algoritmo
            if (vertexCost[v1] == INT_MAX) { break; }
            
            // Adicionando na árvore
            inTree[v1] = true;
            
            // Percorrendo toda a lista de arestas
            EdgeNode * edge = m_edges[v1];
            while(edge) {
                vertex v2 = edge->otherVertex();
                int cost = edge->cost();
                // Se estivermos analisando um vértice que não está árvore
                // e cujo custo a partir de v1 é menor que o custo corrente,
                // atualizamos seu custo, definimos v1 como pai e colocamos
                // esse custo na fila prioritária
                if (!inTree[v2] && cost < vertexCost[v2]) {
                    vertexCost[v2] = cost;
                    parent[v2] = v1;
                    heap.insert_or_update(vertexCost[v2], v2);
                }
                
                edge = edge->next();
            }
        }
    }
};