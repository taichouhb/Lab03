package edu.cs377.java;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Main {
    public static void main(String[] args) throws URISyntaxException, IOException {
        if(args.length != 1) {
            System.out.println("File parameter needed");
            System.exit(0);
        }

        BufferedReader lines = Files.newBufferedReader(Paths.get(Main.class.getResource(args[0]).toURI()));

        String line;
        int index = 0;
        while ((line = lines.readLine()) != null) {
            System.out.println(line);
            index++;
        }
    }
}
