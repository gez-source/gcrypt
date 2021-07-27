using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    public class Program
    {
        const string charsetLowerUpperAlphaNumeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
        const string charsetLowerAlphaNumeric = "abcdefghijklmnopqrstuvwxyz";
        const string charset = charsetLowerAlphaNumeric;

        public static void Main(string[] args)
        {
            Console.WriteLine(GCrypt.Hash("The quick brown fox jumps over the lazy dog"));
            Console.WriteLine(GCrypt.Hash("The quick brown fox jumps over the lazy dog."));
            Console.WriteLine(GCrypt.Hash("the quick brown fox jumps over the lazy dog"));
            Console.WriteLine(GCrypt.Hash(""));
            //Console.WriteLine(GCrypt.HashFile("C:\\dev\\utas\\programming principles\\Assignment 3 - Development Project\\python-3.9.5-amd64.exe"));

            // Simple bruteforce:
            //string o = "";
            //for (int a = 0; a < charset.Length; a++)
            //{
            //    for (int b = 0; b < charset.Length; b++)
            //    {
            //        for (int c = 0; c < charset.Length; c++)
            //        {
            //            o = "" + charset[a] + charset[b] + charset[c];

            //            Console.WriteLine(o + " " + GCrypt.Hash(o));

            //        }
            //    }
            //}

            // Find collision:
            //string collision_with = "cat";
            //string target_hash = GCrypt.Hash(collision_with);

            //Bruteforce((string attempt) =>
            //{
            //    string source_hash = GCrypt.Hash(attempt);

            //    Console.Write(attempt + " " + source_hash);

            //    if (attempt != collision_with && target_hash == source_hash)
            //    {
            //        // Found collision.
            //        Console.Write(" COLLISION FOUND");
            //        return true;
            //    }

            //    Console.Write("\n");

            //    return false;
            //});

            Console.ReadLine();
        }
    }
}
