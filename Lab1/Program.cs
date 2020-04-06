using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace Lab1
{
    class Program
    {
        static string[] SearchDirectory(string path)
        {
            string[] resultSearch = Directory.GetDirectories(path);
            return resultSearch;
        }

        static string[] SearchFiles(string path, string pattern)
        {
            string[] resultSearch = Directory.GetFiles(path, pattern, SearchOption.AllDirectories);
            return resultSearch;
        }

        static void WriteResult(string resultFile, List<string> all_files)
        {
            List<string> resultText = new List<string>();
            foreach (string file in all_files)
            {
                resultText.AddRange(File.ReadAllLines(file));

            }

            File.WriteAllLines(resultFile, resultText);
        }

        static List<string> FoundFiles(string[] res, string path_for_search, string fileName)
        {
            string pattern = "*" + fileName + "*" + "*.txt";
            string print_res = "На диске " + path_for_search + " нашли следующие файлы :\n";
            string[] kek = Directory.GetFiles(path_for_search, pattern);
            List<string> all_files = new List<string>();
            foreach (string lol in kek)
            {
                print_res += lol + "\n";
                all_files.Add(lol);
            }

            foreach (string folder in res)
            {
                try
                {
                    string[] files = SearchFiles(folder, pattern);
                    foreach (string file in files)
                    {
                        print_res += file + "\n";
                        all_files.Add(file);
                    }
                }
                catch
                {

                    Console.WriteLine("!!!!! Ошибка доступа к директории: " + folder);
                }
            }
            Console.WriteLine(print_res);
            return all_files;
        }
        static void Main(string[] args)
        {
            var kek = Console.ReadLine();
            Console.WriteLine(@"Начинаем поиск на диске D:\\");
            Console.WriteLine("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
            string path_for_search = @"C:\Users\Nikita\source\repos\OSS\Lab1\test";
            string resultFile = @"C:\Users\Nikita\source\repos\OSS\Lab1\result.txt";
            string[] res = SearchDirectory(path_for_search);
            List<string> all_files = FoundFiles(res, path_for_search, kek);
            WriteResult(resultFile, all_files);
            Console.ReadKey();
        }
    }
}
