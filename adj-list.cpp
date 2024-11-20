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
};