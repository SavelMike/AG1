package Graphs;

import java.util.ArrayList;
import java.util.Scanner;

public class TestGraph {
    public static void main(String[] args) throws Exception {
        Graph graph = new Graph();

        while(true) {
            System.out.println("Menu:\n1 to add edge");
            System.out.println("2 to add vertex");
            System.out.println("3 to build graph");
            System.out.println("4 to print graph");
            System.out.println("5 to find source");
            System.out.println("6 to delete source");
            System.out.println("7 to find all sources");
            System.out.println("8 for TopSort");
            System.out.println("0 to exit");
            Scanner scanner = new Scanner(System.in);
            int opt = scanner.nextInt();
            switch (opt) {
                case 1:
                    System.out.println("Type in begin vertex and end vertex:");
                    int begin = scanner.nextInt();
                    int end = scanner.nextInt();
                    graph.addEdge(begin, end);
                    break;
                case 2:
                    System.out.println("Type in new vertex:");
                    int vertex = scanner.nextInt();
                    graph.addVertex(vertex);
                    break;
                case 3:
                    graph.clear();
                    System.out.println("Input head vertex and adjacent vertices: ");
                    scanner.nextLine();
                    graph.readGraph(scanner);
                    break;
                case 4:
                    graph.printGraph();
                    break;
                case 5:
                    graph.findSourse();
                    break;
                case 6:
                    graph.deleteSourse(4);
                    break;
                case 7:
                    System.out.println(graph.findAllSources());
                    break;
                case 8:
                    graph.topSort();
                    break;
                case 9:
                    System.exit(1);
            }
        }
    }
}
