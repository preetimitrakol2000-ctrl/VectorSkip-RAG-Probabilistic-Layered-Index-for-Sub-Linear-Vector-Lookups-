from vector_bridge import VectorSkipBridge

def main():
    kb_mapping = {
        101: "Playbook Alpha: Block incoming ports if a multi-stage brute force SSH authentication pattern maps across network targets.",
        102: "Playbook Beta: Revoke API Bearer Tokens instantly if dynamic payload formats encounter unhandled template interpolation escapes."
    }

    index = VectorSkipBridge()
    # Mocking precomputed text embeddings of knowledge base documents
    index.index_document(doc_id=101, embedding=[1.2f, 0.4f, 0.9f, 2.1f])
    index.index_document(doc_id=102, embedding=[4.5f, 3.1f, 0.2f, 0.8f])

    # Attack alert vector isolated from telemetry
    incident_query_vector = [1.1f, 0.5f, 0.8f, 2.0f]
    matched_id = index.search_nearest(incident_query_vector)

    print("=== VECTORSKIP-RAG ENGINE OPTIMIZER ===")
    print(f" -> Nearest Document Segment ID located via C SkipList: {matched_id}")
    print(f" -> Extracted RAG Playbook Context:\n{kb_mapping.get(matched_id, 'No context found.')}")

if __name__ == "__main__":
    main()
