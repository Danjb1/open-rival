# Lobby Flow

## Concepts

- The host is responsible for starting the relay server.
- From the server's point of view, everyone is a client (including the host).
- The server assigns a unique client ID to every connection.
- A player ID specifies which player a client controls.
- Players maintain a map of client ID -> client info (player ID and name).
- Clients send packets to the server, which forwards them to all other clients.
    - Upon receiving a packet, `Connection` will either deserialize it (clients) or wrap it in a `RelayedPacket` (server).

## Host Entry

- The host starts the server.
- The server starts listening for new client connections.
- The host then immediately connects to the server.
- The host is always assigned client ID 0.
- The host enters the lobby and claims player ID 0.
- Up to this point, the host has not sent any packets, since there are no other players to communicate with.

## Client Entry

- Client connects to server and sends a `RequestJoinPacket` with their name.
- Host assigns them a player ID.
- Host sends an `AcceptPlayerPacket` with the player ID assignment (to all clients).
- Host sends a `LobbyWelcomePacket` containing the current lobby state.
    - This is sent to everyone but ignored by exising clients.

## Start Game

- Host sends a `StartGamePacket`.
- All players advance to the GameState.
- By now, all players should have the ClientInfo of every other player.
