# CentOS Image
FROM centos:latest

#Create directory for sockets and to send images
RUN mkdir -p /app/Sockets/To Send
# Copy exec & To Send directory
COPY Client /app/

# Set the working directory inside the container
WORKDIR /app/

# Define the command to run your executable
CMD ["./Client"]
