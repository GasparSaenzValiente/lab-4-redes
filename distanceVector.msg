struct DistanceEntry {
    int destination;
    int cost;
}
message DistanceVectorMsg {
    int senderId;
    DistanceEntry distanceVector[];
}
