package Graphs;

import java.util.ArrayList;
import java.util.Scanner;

//
// Directed graph
//
public class Graph {
    private ArrayList<ArrayList<Integer>> adjList;

    // default constructor
    public Graph() {
        adjList = new ArrayList<ArrayList<Integer>>();
    }

    // output graph's adajcency list
    public void printGraph() {
        System.out.println("num of elements: " + adjList.size());
        for (ArrayList<Integer> integers : adjList) {
            for (Integer vertex : integers) {
                System.out.print(vertex + " ");
            }
            System.out.println();
        }
    }

    // Read standard input, build graph object
    // expected input:
    // 1 2 3
    // 4 5 6
    // where first number is vertex, others are adjacent vertices
    // TO DO  input validity control is missing
    public void readGraph(Scanner scanner) {
        while (scanner.hasNextLine()) {
            String input = scanner.nextLine();

            Scanner strScan = new Scanner(input);
            if (!strScan.hasNextInt()) {
                break;
            }
            int input2 = strScan.nextInt();
            // Look and add if not found
            int index = search(input2);
            while (strScan.hasNextInt()) {
                int num = strScan.nextInt();
                search(num);
                adjList.get(index).add(num);
            }
        }
    }

    // Input:
    //      Integer begin
    //      Integer end
    // Task:
    //      add new edge between vertices
    //      check for existing edge and non-existing vertices
    public void addEdge(Integer begin, Integer end) throws Exception {
        boolean foundBegin = false;
        boolean foundEnd = false;
        // Make sure begin and end are in the graph
        for (ArrayList<Integer> integers : adjList) {
            if (integers.get(0).equals(begin)) {
                if (foundBegin) {
                    // Already seen
                    throw new Exception();
                }
                foundBegin = true;
                for (int i = 1; i < integers.size(); i++) {
                    if (integers.get(i).equals(end)) {
                        System.out.println("Edge " + begin + " -> " + end + " exists");
                        return;
                    }
                }
            }
            if (integers.get(0).equals(end)) {
                if (foundEnd)
                    // Already seen
                    throw new Exception();
                foundEnd = true;
            }
        }

        if (foundBegin && foundEnd) {
            int index = search(begin);
            adjList.get(index).add(end);
        } else {
            System.out.println("There is no vertex " + begin + " or " + end);
        }
    }

    // Input:
    //     Integer vertex
    // Task:
    //     adding vertex
    //     check for already existing vertex
    public void addVertex(Integer vertex) {
        int listSize = adjList.size();
        search(vertex);
        if (listSize == adjList.size()) {
            System.out.println("Vertex already exists");
        } else {
            System.out.println("Vertex " + vertex + " is added");
        }
    }

    // search for vertex in graph
    // if not found add the vertex into graph
    // return value: index of the vertex in adjList
    private int search(int vertex) {
        int index = 0;
        for (ArrayList<Integer> integers : adjList) {
            if (vertex == integers.get(0)) {
                break;
            }
            index++;
        }
        if (index == adjList.size()) {   // input2 is not found
            ArrayList<Integer> arr = new ArrayList<>();
            arr.add(vertex);
            adjList.add(arr);
        }

        return index;
    }

    // not ready
    public void topSort() throws Exception {
        Graph copy = this;
        while (true) {
            Integer res = copy.findSourse();
            if (res == null) {
                if (copy.adjList.size() > 0)
                    System.out.println("Graph is cyclic");
                break;
            }
            System.out.print(res + " ");
            copy.deleteSourse(res);
        }
    }

    // helper for findSource and findAllSources
    private boolean isSource(int vertex) {
        for (ArrayList<Integer> integers : adjList) {
            for (int k = 1; k < integers.size(); k++) {
                if (integers.get(k) == vertex)
                    return false;
            }
        }
        return true;
    }

    // find source of a graph
    // return value: first source on the way
    // if there is no source, throw exception
    public Integer findSourse() throws Exception {
        for (ArrayList<Integer> integers : adjList) {
            if (isSource(integers.get(0))) {
                return integers.get(0);
            }
        }
        // there is not source, cyclic graph
        return null;
    }

    // find all sources of a graph
    // return value: array list of sources
    // if there is no source, throw exception
    public ArrayList<Integer> findAllSources() throws Exception {
        ArrayList<Integer> res = new ArrayList<>();

        for (ArrayList<Integer> integers : adjList) {
            if (isSource(integers.get(0))) {
                res.add(integers.get(0));
            }
        }

        if (res.size() == 0)
            // there is not source, cyclic graph
            throw new Exception();
        return res;
    }

    // Input:
    //      vertex: source
    // Task:
    //      remove adjacency list from the source
    // If vertex is not a source throw Exception
    public void deleteSourse(int vertex) throws Exception {
        if (!isSource(vertex)) {
            throw new Exception();
        }
        for (int i = 0; i < adjList.size(); i++) {
            if (adjList.get(i).get(0).equals(vertex)) {
                adjList.remove(i);
                return;
            }
        }

        throw new Exception();
    }

    public void clear() {
        adjList.clear();
    }
}
