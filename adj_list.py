class EdgeNode:
    def __init__(self, other_vertex, next_edge=None):
        self._other_vertex = other_vertex
        self._next = next_edge

    def other_vertex(self):
        return self._other_vertex

    def next(self):
        return self._next

    def set_next(self, next_edge):
        self._next = next_edge


class GraphAdjList:
    def __init__(self, num_vertices):
        self._num_vertices = num_vertices
        self._num_edges = 0
        self._edges = [None] * num_vertices  # Initialize a list to hold the edges for each vertex

    def add_edge(self, v1, v2):
        # Check if the edge already exists
        edge = self._edges[v1]
        while edge:
            if edge.other_vertex() == v2:
                return  # The edge already exists
            edge = edge.next()

        # Add a new edge
        new_edge = EdgeNode(v2, self._edges[v1])
        self._edges[v1] = new_edge
        self._num_edges += 1

    def remove_edge(self, v1, v2):
        edge = self._edges[v1]
        prev_edge = None
        while edge:
            if edge.other_vertex() == v2:
                # Found the edge to remove
                if prev_edge:
                    prev_edge.set_next(edge.next())  # Remove the edge by updating the previous edge's next
                else:
                    self._edges[v1] = edge.next()  # Update the first edge if it's the one to be removed
                self._num_edges -= 1
                break
            prev_edge = edge
            edge = edge.next()

    def print(self):
        for i in range(self._num_vertices):
            edge = self._edges[i]
            print(f"Vertex {i}: ", end="")
            while edge:
                print(f"{edge.other_vertex()} -> ", end="")
                edge = edge.next()
            print("None")

    def has_topological_order(self):
        in_degree = [0] * self._num_vertices

        # Calculate the in-degree for each vertex
        for i in range(self._num_vertices):
            edge = self._edges[i]
            while edge:
                in_degree[edge.other_vertex()] += 1
                edge = edge.next()

        # Perform topological sort using Kahn's algorithm
        queue = []
        for i in range(self._num_vertices):
            if in_degree[i] == 0:
                queue.append(i)

        order = [-1] * self._num_vertices
        start = 0
        counter = 0

        while start < len(queue):
            v = queue[start]
            start += 1
            order[v] = counter
            counter += 1

            edge = self._edges[v]
            while edge:
                other = edge.other_vertex()
                in_degree[other] -= 1
                if in_degree[other] == 0:
                    queue.append(other)
                edge = edge.next()

        # Check if a topological ordering is possible (i.e., all vertices must be visited)
        return counter == self._num_vertices
