#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace progressive {

// ==== Directed Graph with Cycle Detection ====
//
// Original Kotlin (GraphUtils.kt:25-166):
//   internal data class GraphNode(val name: String)
//   internal data class GraphEdge(val source: GraphNode, val destination: GraphNode)
//   internal class Graph { adjacencyList, getOrCreateNode, addEdge, edgesOf,
//       findBackwardEdges (3-color iterative DFS), flattenDestination (transitive closure),
//       withoutEdges (graph copy without specified edges), toString }

struct GraphNode {
    std::string name;

    bool operator==(const GraphNode& other) const { return name == other.name; }
    bool operator<(const GraphNode& other) const { return name < other.name; }
};

struct GraphEdge {
    GraphNode source;
    GraphNode destination;

    bool operator==(const GraphEdge& other) const {
        return source.name == other.source.name
            && destination.name == other.destination.name;
    }
};

// Hash helpers for unordered containers
struct GraphNodeHash {
    size_t operator()(const GraphNode& n) const {
        return std::hash<std::string>{}(n.name);
    }
};

struct GraphEdgeHash {
    size_t operator()(const GraphEdge& e) const {
        return std::hash<std::string>{}(e.source.name)
            ^ (std::hash<std::string>{}(e.destination.name) << 1);
    }
};

class Graph {
    std::unordered_map<GraphNode, std::vector<GraphEdge>, GraphNodeHash> adjacencyList_;

public:
    // Original Kotlin: fun getOrCreateNode(name: String): GraphNode
    GraphNode getOrCreateNode(const std::string& name);

    // Original Kotlin: fun addEdge(sourceName, destinationName)
    void addEdge(const std::string& sourceName, const std::string& destinationName);

    // Original Kotlin: fun addEdge(source: GraphNode, destination: GraphNode)
    void addEdge(const GraphNode& source, const GraphNode& destination);

    // Original Kotlin: fun edgesOf(node: GraphNode): List<GraphEdge>
    std::vector<GraphEdge> edgesOf(const GraphNode& node) const;

    // Original Kotlin: fun withoutEdges(edgesToPrune): Graph
    Graph withoutEdges(const std::vector<GraphEdge>& edgesToPrune) const;

    // Original Kotlin: fun findBackwardEdges(startFrom?): List<GraphEdge>
    // Iterative DFS with 3-color marking: -1=notVisited, 0=inPath, 1=completed
    // Detects ALL cycles in the graph (including disconnected components/forests)
    std::vector<GraphEdge> findBackwardEdges(const GraphNode* startFrom = nullptr);

    // Original Kotlin: fun flattenDestination(): Map<GraphNode, Set<GraphNode>>
    // Transitive closure — for each vertex, returns ALL reachable nodes
    // Only call on acyclic graph!
    std::unordered_map<GraphNode, std::unordered_set<GraphNode, GraphNodeHash>, GraphNodeHash>
    flattenDestination() const;

    // For debugging
    std::string toString() const;
};

// ==== Best Chunk Size ====
//
// Original Kotlin (BestChunkSize.kt:21-44):
//   data class BestChunkSize(numberOfChunks: Int, chunkSize: Int)
//   fun computeBestChunkSize(listSize: Int, limit: Int): BestChunkSize

struct BestChunkSize {
    int numberOfChunks = 1;
    int chunkSize = 0;

    bool shouldChunk() const { return numberOfChunks > 1; }
};

// Original Kotlin: ceil division to minimize API calls
inline BestChunkSize computeBestChunkSize(int listSize, int limit) {
    if (listSize <= limit) return {1, listSize};
    int numberOfChunks = (listSize + limit - 1) / limit; // ceil division
    int chunkSize = (listSize + numberOfChunks - 1) / numberOfChunks;
    return {numberOfChunks, chunkSize};
}

// ==== Glob → Regex Converter ====
//
// Original Kotlin (Glob.kt:21-39):
//   fun String.hasSpecialGlobChar(): Boolean
//   fun String.simpleGlobToRegExp(): String
//
// Converts simple glob patterns (*, ?) to regex:
//   * → .*
//   ? → .
//   . → \\.
//   \ → \\\\

inline bool hasSpecialGlobChar(const std::string& pattern) {
    return pattern.find('*') != std::string::npos
        || pattern.find('?') != std::string::npos;
}

inline std::string simpleGlobToRegExp(const std::string& pattern) {
    std::string result;
    for (char c : pattern) {
        switch (c) {
            case '*': result += ".*"; break;
            case '?': result += "."; break;
            case '.': result += "\\."; break;
            case '\\': result += "\\\\"; break;
            default: result += c;
        }
    }
    return result;
}

// ==== Base64 URL Conversion ====
//
// Original Kotlin (Base64.kt:21-38):
//   fun base64UrlToBase64(base64Url): String
//   fun base64ToBase64Url(base64): String
//   fun base64ToUnpaddedBase64(base64): String

inline std::string base64UrlToBase64(const std::string& base64Url) {
    std::string result = base64Url;
    for (char& c : result) {
        if (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    return result;
}

inline std::string base64ToBase64Url(const std::string& base64) {
    std::string result;
    for (char c : base64) {
        if (c == '\n') continue;
        if (c == '+') result += '-';
        else if (c == '/') result += '_';
        else if (c == '=') continue;
        else result += c;
    }
    return result;
}

inline std::string base64ToUnpaddedBase64(const std::string& base64) {
    std::string result;
    for (char c : base64) {
        if (c == '\n') continue;
        if (c == '=') continue;
        result += c;
    }
    return result;
}

} // namespace progressive
