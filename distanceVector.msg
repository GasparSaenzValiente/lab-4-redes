struct DistanceEntry {
    int destination;
    int cost;
}
message DistanceVector {
    int senderId;
    DistanceEntry entries[];
}
