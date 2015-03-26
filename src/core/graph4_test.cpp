#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


int main(int, char *[]) {
  using namespace boost;
  typedef property < vertex_name_t, char >Name;
  typedef property < vertex_index_t, std::size_t, Name > Index;
  typedef adjacency_list < listS, listS, directedS, Index > graph_t;
  typedef graph_traits < graph_t >::vertex_descriptor vertex_t;
  graph_t G;
  std::vector < vertex_t > verts(4);
  for (int i = 0; i < 4; ++i) {
    verts[i] = add_vertex(Index(i, Name('a' + i)), G);
  }

  add_edge(verts[1], verts[2], G);
  add_edge(verts[1], verts[3], G);
  add_edge(verts[2], verts[1], G);
  add_edge(verts[3], verts[2], G);
  add_edge(verts[3], verts[0], G);

  std::cout << "Graph G:" << std::endl;
  print_graph(G, get(vertex_name, G));

  adjacency_list <> TC;
  transitive_closure(G, TC);

  std::cout << std::endl << "Graph G+:" << std::endl;
  char name[] = "abcd";
  print_graph(TC, name);
  std::cout << std::endl;

  std::ofstream out("tc-out.dot");
  write_graphviz(out, TC, make_label_writer(name));

  return 0;
}
