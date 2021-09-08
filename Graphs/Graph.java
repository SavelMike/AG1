package Graphs;

import java.util.ArrayList;
import java.util.Scanner;

public class Graph {
    private ArrayList<ArrayList<Integer>> adjList;

    public Graph() {
        adjList = new ArrayList<ArrayList<Integer>>();
    }

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
    public void readGraph() {
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextLine()) {
            String input = scanner.nextLine();

            Scanner strScan = new Scanner(input);
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
}
