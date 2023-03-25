#pragma once

#include <list>
#include <iostream>
#include <vector>

#include <unordered_map>


template<class node_type, class edge_type>
class Node;

template<class node_type, class edge_type>
class Edge {
    using node_t = Node<node_type, edge_type>;

    edge_type data_;
    node_t &source_, &destination_;

    public:
        Edge(node_t &source, node_t &destination, edge_type data) : source_(source), destination_(destination), data_(data) {}

        node_t &GetSource() const {
            return source_;
        }
        node_t &GetDestination() const {
            return destination_;
        }
        const edge_type &GetData() const {
            return data_;
        }
};

template<class node_type, class edge_type>
class Node {
    using edges_list = std::list<Edge<node_type, edge_type>>;
    using edge_iter = typename edges_list::iterator;

    node_type data_;
    std::list<edge_iter> input_;
    edges_list output_;

    public:
        Node() noexcept : data_(), input_(), output_() {}
        Node(node_type data) noexcept : data_(data), input_(), output_() {}

        void SetData(node_type &data) {
            data_ = data;
        }
        edge_iter AddOutput(Node &destination, edge_type edge_data) {
            output_.emplace_front(*this, destination, edge_data);
            return output_.begin();
        }
        void AddInput(edge_iter iter) {
            input_.push_front(iter);
        }
        const node_type &GetData() const {
            return data_;
        }

        void Print() const {
            std::cout << "Node data = " << data_ << ":\nInput from nodes & edge data: ";
            for (auto &in : input_) {
                std:: cout << "'" << in->GetSource().data_ << "'" << " & " << in->GetData() << ", ";
            }
            std::cout << "\nOutput to nodes & edge data: ";
            for (auto &out : output_) {
                std:: cout << "'" <<  out.GetDestination().data_ << "'" << " & " << out.GetData() << ", ";
            }
            std::cout << "\n";
        }
};


template<class node_type, class edge_type>
class Graph {
    public:
        using node_t = Node<node_type, edge_type>;

    private:
        std::vector<std::list<node_t>> groups_;

    public:
        Graph(int groups_count = 1) noexcept : groups_(groups_count) {}

        node_t &AddNode(node_type node_data, int group = 0) {
            groups_[group].emplace_back(node_data);
            return groups_[group].back();
        }

        void SetData(node_t &node, node_type node_data) {
            node.SetData(node_data);
        }

        virtual void AddEdge(node_t &source, node_t &destination, edge_type edge_data) = 0;

        std::list<node_t> &GetNodes(int group = 0) {
            return groups_[group];
        }

        node_t &GetNode(node_type for_find) {
            for (auto &group : groups_) {
                for (auto &node : group) {
                    if (node.GetData() == for_find) {
                        return node;
                    }
                }
            }
            throw std::invalid_argument("Node not found");
        }

        void Print() const {
            for (int k = 0; k < groups_.size(); ++k) {
                std::cout << "group " << k << "\n";
                for (auto &node : groups_[k]) {
                    node.Print();
                    std::cout << "\n";
                }
            }
        }
};

template<class node_type, class edge_type>
class DirectedGraph : public Graph<node_type, edge_type> {
    public:
        using node_t = typename Graph<node_type, edge_type>::node_t;

        DirectedGraph(int groups_count = 1) noexcept : Graph<node_type, edge_type>(groups_count) {}

        void AddEdge(const node_type source, const node_type destination, edge_type edge_data) {
            AddEdge(this->GetNode(source), this->GetNode(destination), edge_data);
        }

        void AddEdge(node_t &source, node_t &destination, edge_type edge_data) override {
            source.AddOutput(destination, edge_data);
        }
};


template<class node_type, class edge_type>
class UndirectedGraph : public Graph<node_type, edge_type> {
    public:
        using node_t = typename Graph<node_type, edge_type>::node_t;

        UndirectedGraph(int groups_count = 1) noexcept : Graph<node_type, edge_type>(groups_count) {}

        void AddEdge(const node_type source, const node_type destination, edge_type edge_data) {
            AddEdge(this->GetNode(source), this->GetNode(destination), edge_data);
        }

        void AddEdge(node_t &source, node_t &destination, edge_type edge_data) override {
            destination.AddInput(source.AddOutput(destination, edge_data));
        }
};
