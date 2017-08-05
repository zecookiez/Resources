package algorithms.graph.shortestpath;

import datastructures.Stack;
import datastructures.graph.WeightedGraph;
import datastructures.graph.WeightedEdge;

/**
 *  The {@code DijkstraUndirectedSP} class represents a data type for solving
 *  the single-source shortest paths problem in edge-weighted graphs
 *  where the edge weights are nonnegative.
 *  <p>
 *  This implementation uses Dijkstra's algorithm with a binary heap.
 *  The constructor takes time proportional to <em>E</em> log <em>V</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the {@code distTo()} and {@code hasPathTo()} methods take
 *  constant time and the {@code pathTo()} method takes time proportional to the
 *  number of edges in the shortest path returned.
 *  <p>
 *  For additional documentation,    
 *  see <a href="http://algs4.cs.princeton.edu/44sp">Section 4.4</a> of    
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *  See {@link DijkstraDirectedSP} for a version on edge-weighted digraphs.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 *  @author Nate Liu
 */
public class ClassicalDijkstraUndirectedSP {
    private double[] distTo;          // distTo[v] = distance  of shortest s->v path
    private WeightedEdge[] edgeTo;            // edgeTo[v] = last edge on shortest s->v path
    private boolean[] marked;

    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every
     * other vertex in the edge-weighted graph {@code G}.
     *
     * @param  G the edge-weighted digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    public ClassicalDijkstraUndirectedSP(WeightedGraph G, int s) {
        distTo = new double[G.V()];
        edgeTo = new WeightedEdge[G.V()];
        marked = new boolean[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        for (int v = 0; v < G.V() - 1; v++) {
            int minV = -1;
            for (int w = 0; w < G.V(); w++) {
                if (!marked[w] && (minV == -1 || distTo[minV] > distTo[w])) minV = w;
            }
            marked[minV] = true;
            for (WeightedEdge e : G.adj(minV)) {
                int w = e.other(minV);
                if (distTo[w] > distTo[minV] + e.weight()) {
                    distTo[w] = distTo[minV] + e.weight();
                    edgeTo[w] = e;
                }
            }
        }
    }
    
    /**
     * Returns the length of a shortest path between the source vertex {@code s} and
     * vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return the length of a shortest path between the source vertex {@code s} and
     *         the vertex {@code v}; {@code Double.POSITIVE_INFINITY} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public double distTo(int v) {
        validateVertex(v);
        return distTo[v];
    }

    /**
     * Returns true if there is a path between the source vertex {@code s} and
     * vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return {@code true} if there is a path between the source vertex
     *         {@code s} to vertex {@code v}; {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean hasPathTo(int v) {
        validateVertex(v);
        return distTo[v] < Double.POSITIVE_INFINITY;
    }

    /**
     * Returns a shortest path between the source vertex {@code s} and vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return a shortest path between the source vertex {@code s} and vertex {@code v};
     *         {@code null} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public Iterable<WeightedEdge> pathTo(int v) {
        validateVertex(v);
        if (!hasPathTo(v)) return null;
        Stack<WeightedEdge> path = new Stack<WeightedEdge>();
        int x = v;
        for (WeightedEdge e = edgeTo[v]; e != null; e = edgeTo[x]) {
            path.push(e);
            x = e.other(x);
        }
        return path;
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = distTo.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }
}