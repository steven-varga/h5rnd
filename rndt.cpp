/*
copyright steven varga, 2021, feb 18, Toronto, ON, Canada;  MIT license
*/

#include "argparse.h"
#include <h5cpp/all>

#include <string>
#include <random>
#include <queue>
#include <algorithm>
#include <fstream>


using edge_t = std::vector<std::pair<unsigned,unsigned>>;
using degree_t = std::vector<unsigned>;

// std::enable_if<std::is_integral<T>::value, void>::type
edge_t prufer2edges(const degree_t& a){
    int N = a.size() + 2;
    edge_t T;
    degree_t degree(N);
    std::fill(std::begin(degree), std::end(degree), 1);
    for(int i:a) degree[i]++; 
    // Next, for each number in the sequence a[i], find the first (lowest-numbered) 
    // node, j, with degree equal to 1, add the edge (j, a[i]) to the tree, and 
    // decrement the degrees of j and a[i]
    for(int i:a) for(int j=0; j<N; j++)
        if(degree[j] == 1) {
            T.push_back(std::make_pair(i,j)), degree[i]--, degree[j]--;
            break;
        }
    // At the end of this loop two nodes with degree 1 will remain (call them u, v).
    // Lastly, add the edge (u,v) to the tree.[3]
    int u=0, v=0;
    for(; degree[u] != 1; u++, v++); // we have `u`
    for(v++; degree[v] != 1; v++);   // `v` is the last...
    T.push_back(std::make_pair(u,v));

    return T;
}


int main(int argc, char **argv) {
    argparse::ArgumentParser arg("rndt", "0.0.1");

    arg.add_argument("-o", "--output").required().help("the new hdf5 will be created/or opened rw");
    arg.add_argument("-d", "--data-sets").help("number of datasets")
        .default_value(100).action([](const std::string& value) { return std::stoi(value); });
    arg.add_argument("-g", "--groups").help("number of groups")
        .default_value(10).action([](const std::string& value) { return std::stoi(value); });
    arg.add_argument("--hdf5").help("flag for HDF5 output")
        .default_value(false).implicit_value(true);
    arg.add_argument("--graphviz").help("flag for graphviz output")
        .default_value(false).implicit_value(true);

    std::string path;
    try {
        arg.parse_args(argc, argv);

        int ngroups = arg.get<int>("--groups") + 1; // compensate for root group
        int ndatasets = arg.get<int>("--data-sets");

        std::random_device rd;
        std::mt19937 src(rd());
        std::uniform_int_distribution<size_t> draw(1, ngroups);
        degree_t prufer_sequence(ndatasets + ngroups - 2);
        std::vector<bool> is_group(prufer_sequence.size());
        
        // initial conditions: 
        std::fill(std::begin(is_group), std::end(is_group), false);
        std::generate(std::begin(prufer_sequence), std::end(prufer_sequence), [&]() -> int {
            auto i = draw(src); is_group[i] = true; return i;
        });
        auto edges = prufer2edges(prufer_sequence);
        int root = edges.back().first;
        
        path = arg.get<std::string>("--output");
        /* GRAPHVIZ OUTPUT */
        if(arg.get<bool>("--graphviz")) {
            std::ofstream os(path, std::ios::out | std::ios::trunc);
            os << "digraph prufer {" << std::endl;
            os << "\tnode [shape=note color=orange fontcolor=purple fontname=\"times bold\" fillcolor=violet width=0.01 height=0.1 fontsize=9.0 labelloc=b style=bold];\n";
            std::cout << "root: " << root <<"\n\n";
            for(int i=0; i < ngroups + 2; i++)
                os <<"\t" << i << " [shape=" << ( i != root  ? "folder color=purple" 
                    : "cylinder color=purple fillcolor=orange style=filled label=HDF5") <<" weight=2.0 width=0.75 height=0.5 fontsize=14.0 labelloc=c]\n";
            os << std::endl << std::endl;
            for(auto e: edges){
                os << e.first << " -> " << e.second <<";" << std::endl;
            }
            os << "}" <<std::endl;
        } else if(arg.get<bool>("--hdf5")) {
        /* HDF5 OUTPUT */
            h5::fd_t fd = h5::create(path, H5F_ACC_TRUNC);
            auto names = h5::utils::get_test_data<std::string>(prufer_sequence.size()+2);
            std::vector<h5::gr_t> gr(ngroups + 2);
            std::vector<double> dataset(1000);

            for(auto v: edges){
                // we can use CAPI routins to test, or just compare gr_t >= 0
                if( is_group[v.second] && !H5Iis_valid(gr[v.second])) 
                    gr[v.second] = h5::gr_t{H5Gcreate(gr[v.first], names[v.second].data(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT)};
                else // it is a leaf node, create a dataset
                    ;//h5::write()
                std::cout <<gr[v.first] <<" " << names[v.first] <<" " << names[v.second] <<"\n";
            }
        }

        
    } catch ( const h5::error::io::file::open& e ){
        std::cerr << "unable to open output HDF5 file: " << path << std::endl; 
    } catch ( const h5::error::io::file::create& e ){
        std::cerr << "unable to create HDF5 file: " << path << std::endl; 
    } catch ( const h5::error::io::file::close& e ){
        std::cerr << "unable to close HDF5 file: " << path << std::endl; 
    } catch ( const std::runtime_error& e ) {
        std::cerr << e.what() << std::endl;
        std::cerr << arg;
    } 
	return 0;
}
