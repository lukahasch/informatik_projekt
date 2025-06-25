### Ziel

Wir nutzen arduinos mit Displays und Tastern, um selbst programmierte Spiele spielen zu können.

Die Spiele sind über ein Menü auswählbar.

### Setup

- es gibt einen arduino (mega), der die zentrale Verarbeitung übernimmt
- es gibt für jeden Spieler (2 Spieler) einen arduino (nano), der das I/O übernimmt und mit dem mega kommuniziert.
- jeder Spieler hat ein Display, das an dem jeweiligen nano hängt
- jeder Spieler hat 5 Taster (up, down, left, right, menu)
