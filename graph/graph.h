#pragma once

#include <list>
#include <iostream>
#include <vector>

#include <unordered_map>

struct func {
    static bool bad_edge(float w) {
        if (w > 1 || w < -1) return true;
        return false;
    }
};


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
        void Update() {
            for (auto iter = input_.begin(); iter != input_.end(); ) {
                if (func::bad_edge((*iter)->GetData())) {
                    (*iter)->GetSource().output_.erase(*iter);
                    iter = input_.erase(iter);
                } else {
                    ++iter;
                }
            }
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

        void AddEdge(node_t &source, node_t &destination, edge_type edge_data) {
            destination.AddInput(source.AddOutput(destination, edge_data));
        }

        std::list<node_t> &GetNodes(int group = 0) {
            return groups_[group];
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

        void Update() {
            for (auto &group: groups_) {
                for (auto &node: group) {
                    node.Update();
                }
            }
        }

};

