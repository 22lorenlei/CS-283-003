1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

In my case, we know that the message is fully received from the server once we get a null byte delimiter. So let's say we get a partial receive. We would call recv over and over again until we encounter a null byte, which is when we know that the message has been fully received. Another way is to send like an expected byte count to the receiver along with the message so that they could keep track of how many bytes they received for recv and call recv again until it reaches it.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Again, we would have to use some sort of delimiter like a \0 to mark the end of a complete message. If we don't do that, then we would not know where or when the message ends. Also, we may overlap a previous command with a current command, and that may cause problems.

3. Describe the general differences between stateful and stateless protocols.

Stateless protocols mean that the server doesn't remember the client's previous sessions. Each request is independent. Stateful protocols mean that the server does remember client's previous sessions, and that since it remembers previous interactions, then the client doesn't have to send context every single time.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is unreliable because there is no guarantee that we will get packet delivered. But, it is faster and more efficient than TCP. So we would use this when we are fine with a little data loss, so like videos.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Sockets, which allow communication between software processes over a network through TCP in our case.