import sys
import re

from pyspark import SparkContext

def flat_map(document):
    """
    Takes in document, which is a key, value pair, where document[0] is the
    document ID and document[1] is the contents of the document.
    """
    """ Your code here. """
    words =  re.findall(r"\w+", document[1])
    words = enumerate(words)
    return [((word,document[0]),[pos]) for pos,word in words]

def map(word_doc_map):
    """ Your code here. """
    return word_doc_map

def reduce(a, b):
    """ Your code here. """

    return a+b

def index(file_name, output="spark-wc-out-index"):
    sc = SparkContext("local[8]", "Index")
    file = sc.wholeTextFiles(file_name)

    indices = file.flatMap(flat_map) \
                  .map(map) \
                  .reduceByKey(reduce)
    indices = indices.sortBy(lambda x: x[0])
    indices.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        index(argv[1])
    else:
        index(argv[1], argv[2])