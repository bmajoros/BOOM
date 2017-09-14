CC		= g++
C		= gcc
DEBUG		= -g
OPTIMIZE	= -O
#CFLAGS		= $(OPTIMIZE) -w -fpermissive -I/gpfs/fs0/data/reddylab/bmajoros/gsl/include
CFLAGS		= $(OPTIMIZE) -w -fpermissive -I$(GSLDIR)/include
LDFLAGS		= $(OPTIMIZE)
BOOM		= .
OBJ		= obj
AR		= ar -s
GSL		= GSL

all:	GSL/BOOM lambda/BOOM libBOOM.a lambda/liblambda.a 

GSL/BOOM:
	cd GSL ; ln -s .. BOOM

lambda/BOOM:
	cd lambda ; ln -s .. BOOM

lambda/liblambda.a:
	cd lambda ; make liblambda.a

.PHONY : clean
clean:
	@rm -f obj/*.o

$(OBJ):
	mkdir $(OBJ)

libBOOM.a:\
	$(OBJ) \
	$(OBJ)/Bed3Record.o \
	$(OBJ)/VcfReader.o \
	$(OBJ)/CombinationIterator.o \
	$(OBJ)/Process.o \
	$(OBJ)/TempFilename.o \
	$(OBJ)/HistogramArray.o \
	$(OBJ)/CodonIterator.o \
	$(OBJ)/Interval.o \
	$(OBJ)/GCF.o \
	$(OBJ)/CigarAlignment.o \
	$(OBJ)/CigarString.o \
	$(OBJ)/SubsetEnumerator.o \
	$(OBJ)/Math.o \
	$(OBJ)/WigBinary.o \
	$(OBJ)/FastcReader.o \
	$(OBJ)/FastbReader.o \
	$(OBJ)/CFasta.o \
	$(OBJ)/SumLogProbs.o \
	$(OBJ)/PowerMean.o \
	$(OBJ)/Semaphore.o \
	$(OBJ)/BandedMatrix3D.o \
	$(OBJ)/HigherOrderAlphabet.o \
	$(OBJ)/IndexMap.o \
	$(OBJ)/GammaDistribution.o \
	$(OBJ)/DirichletDistribution.o \
	$(OBJ)/Essex.o \
	$(OBJ)/MolecularSequenceType.o \
	$(OBJ)/ElasVector.o \
	$(OBJ)/L_BFGS_B.o \
	$(OBJ)/regex.o \
	$(OBJ)/AdjacencyGraph.o \
	$(OBJ)/Alignment.o \
	$(OBJ)/AlignmentSeq.o \
	$(OBJ)/AlignmentTrack.o \
	$(OBJ)/GapPatternAlphaMap.o \
	$(OBJ)/GapPatternAlphabet.o \
	$(OBJ)/BinaryAlphabet.o \
	$(OBJ)/Alphabet.o \
	$(OBJ)/AlphabetMap.o \
	$(OBJ)/AminoAlphabet.o \
	$(OBJ)/BinomialDistribution.o \
	$(OBJ)/BitSet.o \
	$(OBJ)/Chi2FitTest.o \
	$(OBJ)/Chi2IndepTest.o \
	$(OBJ)/Chi2Table.o \
	$(OBJ)/CommandLine.o \
	$(OBJ)/ComplexNum.o \
	$(OBJ)/ConfigFile.o \
	$(OBJ)/ContingencyTbl.o \
	$(OBJ)/Correlation.o \
	$(OBJ)/Cypher.o \
	$(OBJ)/DblMatrix.o \
	$(OBJ)/DegenerateDnaAlphabet.o \
	$(OBJ)/DnaAlphabet.o \
	$(OBJ)/DnaDashDotAlphabet.o \
	$(OBJ)/DnaDashAlphabet.o \
	$(OBJ)/DnaAlphabetWithGaps.o \
	$(OBJ)/DnaSubstitutionMatrix.o \
	$(OBJ)/EdgeWeights.o \
	$(OBJ)/EdgeWeightsMatrix.o \
	$(OBJ)/Entropy.o \
	$(OBJ)/Environment.o \
	$(OBJ)/Exceptions.o \
	$(OBJ)/ExtremeValueDistribution.o \
	$(OBJ)/Factorial.o \
	$(OBJ)/FastaReader.o \
	$(OBJ)/FastaWriter.o \
	$(OBJ)/File.o \
	$(OBJ)/FixedSizePriorityQueue.o \
	$(OBJ)/GenericToken.o \
	$(OBJ)/GenericTokenStream.o \
	$(OBJ)/GenericScanner.o \
	$(OBJ)/GaussJordan.o \
	$(OBJ)/GffExon.o \
	$(OBJ)/GffFeature.o \
	$(OBJ)/GffReader.o \
	$(OBJ)/GffTranscript.o \
	$(OBJ)/GffGene.o \
	$(OBJ)/Graph.o \
	$(OBJ)/Hashpjw.o \
	$(OBJ)/HugeFastaReader.o \
	$(OBJ)/IncidenceGraph.o \
	$(OBJ)/IndexedFasta.o \
	$(OBJ)/InversionMatrix.o \
	$(OBJ)/Iterator.o \
	$(OBJ)/JointDistr.o \
	$(OBJ)/LinRegressor.o \
	$(OBJ)/LinearFunc.o \
	$(OBJ)/MemStream.o \
	$(OBJ)/MemoryProfiler.o \
	$(OBJ)/MinimalSpanningTree.o \
	$(OBJ)/MultRegress.o \
	$(OBJ)/MultiAlignment.o \
	$(OBJ)/MultSeqAlignment.o \
	$(OBJ)/MutInfo.o \
	$(OBJ)/Mutex.o \
	$(OBJ)/NormalDistribution.o \
	$(OBJ)/NthOrderStringIterator.o \
	$(OBJ)/Pipe.o \
	$(OBJ)/PoissonDistribution.o \
	$(OBJ)/Progress.o \
	$(OBJ)/ProteinTrans.o \
	$(OBJ)/PureDnaAlphabet.o \
	$(OBJ)/Random.o \
	$(OBJ)/GslRandom.o \
	$(OBJ)/RealVector.o \
	$(OBJ)/Regex.o \
	$(OBJ)/RouletteWheel.o \
	$(OBJ)/Sequence.o \
	$(OBJ)/SequenceEntropy.o \
	$(OBJ)/SparseGraph.o \
	$(OBJ)/Stacktrace.o \
	$(OBJ)/StrTokenizer.o \
	$(OBJ)/String.o \
	$(OBJ)/SummaryStats.o \
	$(OBJ)/Symbol.o \
	$(OBJ)/Thread.o \
	$(OBJ)/Time.o \
	$(OBJ)/GslMatrix.o \
	$(OBJ)/GslTransitionMatrix.o \
	$(OBJ)/GslVector.o \
	$(OBJ)/GslPermutation.o \
	$(OBJ)/LabeledMatrixLoader.o \
	$(OBJ)/Optimizer.o \
	$(OBJ)/GslContinuousDist.o \
	$(OBJ)/GslGaussian.o \
	$(OBJ)/GslMultiGauss.o \
	$(OBJ)/GslGaussianMixture.o \
	$(OBJ)/GslMultivariateDistribution.o \
	$(OBJ)/TwoBitFastaFile.o 
		$(AR) -r libBOOM.a \
	$(OBJ)/Bed3Record.o \
	$(OBJ)/VcfReader.o \
	$(OBJ)/CombinationIterator.o \
	$(OBJ)/TempFilename.o \
	$(OBJ)/Process.o \
	$(OBJ)/HistogramArray.o \
	$(OBJ)/CodonIterator.o \
	$(OBJ)/Interval.o \
	$(OBJ)/GCF.o \
	$(OBJ)/CigarAlignment.o \
	$(OBJ)/CigarString.o \
	$(OBJ)/SubsetEnumerator.o \
	$(OBJ)/Math.o \
	$(OBJ)/WigBinary.o \
	$(OBJ)/FastcReader.o \
	$(OBJ)/FastbReader.o \
	$(OBJ)/CFasta.o \
	$(OBJ)/BinaryAlphabet.o \
	$(OBJ)/SumLogProbs.o \
	$(OBJ)/PowerMean.o \
	$(OBJ)/Semaphore.o \
	$(OBJ)/BandedMatrix3D.o \
	$(OBJ)/GenericToken.o \
	$(OBJ)/GenericTokenStream.o \
	$(OBJ)/GenericScanner.o \
	$(OBJ)/HigherOrderAlphabet.o \
	$(OBJ)/IndexMap.o \
	$(OBJ)/GammaDistribution.o \
	$(OBJ)/DirichletDistribution.o \
	$(OBJ)/Essex.o \
	$(OBJ)/MolecularSequenceType.o \
	$(OBJ)/ElasVector.o \
	$(OBJ)/DnaDashAlphabet.o \
	$(OBJ)/DnaDashDotAlphabet.o \
	$(OBJ)/L_BFGS_B.o \
	$(OBJ)/regex.o \
	$(OBJ)/AdjacencyGraph.o \
	$(OBJ)/Alignment.o \
	$(OBJ)/AlignmentSeq.o \
	$(OBJ)/AlignmentTrack.o \
	$(OBJ)/Alphabet.o \
	$(OBJ)/GapPatternAlphaMap.o \
	$(OBJ)/GapPatternAlphabet.o \
	$(OBJ)/AlphabetMap.o \
	$(OBJ)/AminoAlphabet.o \
	$(OBJ)/BinomialDistribution.o \
	$(OBJ)/BitSet.o \
	$(OBJ)/Chi2FitTest.o \
	$(OBJ)/Chi2IndepTest.o \
	$(OBJ)/Chi2Table.o \
	$(OBJ)/CommandLine.o \
	$(OBJ)/ComplexNum.o \
	$(OBJ)/ConfigFile.o \
	$(OBJ)/ContingencyTbl.o \
	$(OBJ)/Correlation.o \
	$(OBJ)/Cypher.o \
	$(OBJ)/DblMatrix.o \
	$(OBJ)/DegenerateDnaAlphabet.o \
	$(OBJ)/DnaAlphabet.o \
	$(OBJ)/DnaAlphabetWithGaps.o \
	$(OBJ)/DnaSubstitutionMatrix.o \
	$(OBJ)/EdgeWeights.o \
	$(OBJ)/EdgeWeightsMatrix.o \
	$(OBJ)/Entropy.o \
	$(OBJ)/Environment.o \
	$(OBJ)/Exceptions.o \
	$(OBJ)/ExtremeValueDistribution.o \
	$(OBJ)/Factorial.o \
	$(OBJ)/FastaReader.o \
	$(OBJ)/FastaWriter.o \
	$(OBJ)/File.o \
	$(OBJ)/FixedSizePriorityQueue.o \
	$(OBJ)/GaussJordan.o \
	$(OBJ)/GffExon.o \
	$(OBJ)/GffFeature.o \
	$(OBJ)/GffReader.o \
	$(OBJ)/GffTranscript.o \
	$(OBJ)/GffGene.o \
	$(OBJ)/Graph.o \
	$(OBJ)/GslPermutation.o \
	$(OBJ)/Hashpjw.o \
	$(OBJ)/HugeFastaReader.o \
	$(OBJ)/IncidenceGraph.o \
	$(OBJ)/IndexedFasta.o \
	$(OBJ)/InversionMatrix.o \
	$(OBJ)/Iterator.o \
	$(OBJ)/JointDistr.o \
	$(OBJ)/LinRegressor.o \
	$(OBJ)/LinearFunc.o \
	$(OBJ)/MemStream.o \
	$(OBJ)/MemoryProfiler.o \
	$(OBJ)/MinimalSpanningTree.o \
	$(OBJ)/MultRegress.o \
	$(OBJ)/MultiAlignment.o \
	$(OBJ)/MultSeqAlignment.o \
	$(OBJ)/MutInfo.o \
	$(OBJ)/Mutex.o \
	$(OBJ)/NormalDistribution.o \
	$(OBJ)/NthOrderStringIterator.o \
	$(OBJ)/Pipe.o \
	$(OBJ)/PoissonDistribution.o \
	$(OBJ)/Progress.o \
	$(OBJ)/ProteinTrans.o \
	$(OBJ)/PureDnaAlphabet.o \
	$(OBJ)/Random.o \
	$(OBJ)/GslRandom.o \
	$(OBJ)/RealVector.o \
	$(OBJ)/Regex.o \
	$(OBJ)/RouletteWheel.o \
	$(OBJ)/Sequence.o \
	$(OBJ)/SequenceEntropy.o \
	$(OBJ)/SparseGraph.o \
	$(OBJ)/Stacktrace.o \
	$(OBJ)/StrTokenizer.o \
	$(OBJ)/String.o \
	$(OBJ)/SummaryStats.o \
	$(OBJ)/Symbol.o \
	$(OBJ)/Thread.o \
	$(OBJ)/Time.o \
	$(OBJ)/GslMatrix.o \
	$(OBJ)/GslTransitionMatrix.o \
	$(OBJ)/GslVector.o \
	$(OBJ)/LabeledMatrixLoader.o \
	$(OBJ)/Optimizer.o \
	$(OBJ)/GslContinuousDist.o \
	$(OBJ)/GslGaussian.o \
	$(OBJ)/GslMultiGauss.o \
	$(OBJ)/GslGaussianMixture.o \
	$(OBJ)/GslMultivariateDistribution.o \
	$(OBJ)/TwoBitFastaFile.o


#---------------------------------------------------------
$(OBJ)/GslPermutation.o:\
		$(GSL)/Permutation.H \
		$(GSL)/Permutation.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslPermutation.o -c \
		$(GSL)/Permutation.C
#---------------------------------------------------------
$(OBJ)/GslRandom.o:\
		$(GSL)/Random.H \
		$(GSL)/Random.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslRandom.o -c \
		$(GSL)/Random.C
#---------------------------------------------------------
$(OBJ)/GslGaussian.o:\
		$(GSL)/GaussianDistribution.H \
		$(GSL)/GaussianDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslGaussian.o -c \
		$(GSL)/GaussianDistribution.C
#---------------------------------------------------------
$(OBJ)/GslMultiGauss.o:\
		$(GSL)/MultiGauss.H \
		$(GSL)/MultiGauss.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslMultiGauss.o -c \
		$(GSL)/MultiGauss.C
#---------------------------------------------------------
$(OBJ)/GslMultivariateDistribution.o:\
		$(GSL)/MultivariateDistribution.H \
		$(GSL)/MultivariateDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslMultivariateDistribution.o -c \
		$(GSL)/MultivariateDistribution.C
#---------------------------------------------------------
$(OBJ)/GammaDistribution.o:\
		$(GSL)/GammaDistribution.H \
		$(GSL)/GammaDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/GammaDistribution.o -c \
		$(GSL)/GammaDistribution.C
#---------------------------------------------------------
$(OBJ)/DirichletDistribution.o:\
		$(GSL)/DirichletDistribution.H \
		$(GSL)/DirichletDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/DirichletDistribution.o -c \
		$(GSL)/DirichletDistribution.C
#---------------------------------------------------------
$(OBJ)/GslContinuousDist.o:\
		$(GSL)/ContinuousDistribution.H \
		$(GSL)/ContinuousDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslContinuousDist.o -c \
		$(GSL)/ContinuousDistribution.C
#---------------------------------------------------------                     
$(OBJ)/LabeledMatrixLoader.o:\
                $(GSL)/LabeledMatrixLoader.H \
                $(GSL)/LabeledMatrixLoader.C
	$(CC) $(CFLAGS) -o $(OBJ)/LabeledMatrixLoader.o -c \
                $(GSL)/LabeledMatrixLoader.C
#---------------------------------------------------------                     
$(OBJ)/GslMatrix.o:\
                $(GSL)/Matrix.H \
                $(GSL)/Matrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslMatrix.o -c \
                $(GSL)/Matrix.C
#---------------------------------------------------------                     
$(OBJ)/GslTransitionMatrix.o:\
                $(GSL)/TransitionMatrix.H \
                $(GSL)/TransitionMatrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslTransitionMatrix.o -c \
                $(GSL)/TransitionMatrix.C
#---------------------------------------------------------                     
$(OBJ)/GslVector.o:\
                $(GSL)/Vector.H \
                $(GSL)/Vector.C
	$(CC) $(CFLAGS) -o $(OBJ)/GslVector.o -c \
                $(GSL)/Vector.C
#---------------------------------------------------------                     
$(OBJ)/Optimizer.o:\
                $(GSL)/Optimizer.H \
                $(GSL)/Optimizer.C
	$(CC) $(CFLAGS) -o $(OBJ)/Optimizer.o -c \
                $(GSL)/Optimizer.C
#---------------------------------------------------------                     
$(OBJ)/L_BFGS_B.o:\
                $(GSL)/L_BFGS_B.H \
                $(GSL)/L_BFGS_B.C
	$(CC) $(CFLAGS) -o $(OBJ)/L_BFGS_B.o -c \
                $(GSL)/L_BFGS_B.C
#---------------------------------------------------------
$(OBJ)/AdjacencyGraph.o:\
		$(BOOM)/AdjacencyGraph.H \
		$(BOOM)/AdjacencyGraph.C
	$(CC) $(CFLAGS) -o $(OBJ)/AdjacencyGraph.o -c \
		$(BOOM)/AdjacencyGraph.C
#---------------------------------------------------------
$(OBJ)/Alignment.o:\
		$(BOOM)/Alignment.H \
		$(BOOM)/Alignment.C
	$(CC) $(CFLAGS) -o $(OBJ)/Alignment.o -c \
		$(BOOM)/Alignment.C
#---------------------------------------------------------
$(OBJ)/WigBinary.o:\
		$(BOOM)/WigBinary.H \
		$(BOOM)/WigBinary.C
	$(CC) $(CFLAGS) -o $(OBJ)/WigBinary.o -c \
		$(BOOM)/WigBinary.C
#---------------------------------------------------------
$(OBJ)/AlignmentSeq.o:\
		$(BOOM)/AlignmentSeq.H \
		$(BOOM)/AlignmentSeq.C
	$(CC) $(CFLAGS) -o $(OBJ)/AlignmentSeq.o -c \
		$(BOOM)/AlignmentSeq.C
#---------------------------------------------------------
$(OBJ)/AlignmentTrack.o:\
		$(BOOM)/AlignmentTrack.H \
		$(BOOM)/AlignmentTrack.C
	$(CC) $(CFLAGS) -o $(OBJ)/AlignmentTrack.o -c \
		$(BOOM)/AlignmentTrack.C
#---------------------------------------------------------
$(OBJ)/GapPatternAlphabet.o:\
		$(BOOM)/GapPatternAlphabet.H \
		$(BOOM)/GapPatternAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/GapPatternAlphabet.o -c \
		$(BOOM)/GapPatternAlphabet.C
#---------------------------------------------------------
$(OBJ)/GapPatternAlphaMap.o:\
		$(BOOM)/GapPatternAlphaMap.H \
		$(BOOM)/GapPatternAlphaMap.C
	$(CC) $(CFLAGS) -o $(OBJ)/GapPatternAlphaMap.o -c \
		$(BOOM)/GapPatternAlphaMap.C
#---------------------------------------------------------
$(OBJ)/Alphabet.o:\
		$(BOOM)/Alphabet.H \
		$(BOOM)/Alphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/Alphabet.o -c \
		$(BOOM)/Alphabet.C
#---------------------------------------------------------
$(OBJ)/AlphabetMap.o:\
		$(BOOM)/AlphabetMap.H \
		$(BOOM)/AlphabetMap.C
	$(CC) $(CFLAGS) -o $(OBJ)/AlphabetMap.o -c \
		$(BOOM)/AlphabetMap.C
#---------------------------------------------------------
$(OBJ)/AminoAlphabet.o:\
		$(BOOM)/AminoAlphabet.H \
		$(BOOM)/AminoAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/AminoAlphabet.o -c \
		$(BOOM)/AminoAlphabet.C
#---------------------------------------------------------
$(OBJ)/ApproximateAlignment.o:\
		$(BOOM)/ApproximateAlignment.H \
		$(BOOM)/ApproximateAlignment.C
	$(CC) $(CFLAGS) -o $(OBJ)/ApproximateAlignment.o -c \
		$(BOOM)/ApproximateAlignment.C
#---------------------------------------------------------
$(OBJ)/BinomialDistribution.o:\
		$(BOOM)/BinomialDistribution.H \
		$(BOOM)/BinomialDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/BinomialDistribution.o -c \
		$(BOOM)/BinomialDistribution.C
#---------------------------------------------------------
$(OBJ)/BitSet.o:\
		$(BOOM)/BitSet.H \
		$(BOOM)/BitSet.C
	$(CC) $(CFLAGS) -o $(OBJ)/BitSet.o -c \
		$(BOOM)/BitSet.C
#---------------------------------------------------------
$(OBJ)/MolecularSequenceType.o:\
		$(BOOM)/MolecularSequenceType.H \
		$(BOOM)/MolecularSequenceType.C
	$(CC) $(CFLAGS) -o $(OBJ)/MolecularSequenceType.o -c \
		$(BOOM)/MolecularSequenceType.C
#---------------------------------------------------------
$(OBJ)/Chi2FitTest.o:\
		$(BOOM)/Chi2FitTest.H \
		$(BOOM)/Chi2FitTest.C
	$(CC) $(CFLAGS) -o $(OBJ)/Chi2FitTest.o -c \
		$(BOOM)/Chi2FitTest.C
#---------------------------------------------------------
$(OBJ)/Chi2IndepTest.o:\
		$(BOOM)/Chi2IndepTest.H \
		$(BOOM)/Chi2IndepTest.C
	$(CC) $(CFLAGS) -o $(OBJ)/Chi2IndepTest.o -c \
		$(BOOM)/Chi2IndepTest.C
#---------------------------------------------------------
$(OBJ)/Chi2Table.o:\
		$(BOOM)/Chi2Table.H \
		$(BOOM)/Chi2Table.C
	$(CC) $(CFLAGS) -o $(OBJ)/Chi2Table.o -c \
		$(BOOM)/Chi2Table.C
#---------------------------------------------------------
$(OBJ)/CommandLine.o:\
		$(BOOM)/CommandLine.H \
		$(BOOM)/CommandLine.C
	$(CC) $(CFLAGS) -o $(OBJ)/CommandLine.o -c \
		$(BOOM)/CommandLine.C
#---------------------------------------------------------
$(OBJ)/ComplexNum.o:\
		$(BOOM)/ComplexNum.H \
		$(BOOM)/ComplexNum.C
	$(CC) $(CFLAGS) -o $(OBJ)/ComplexNum.o -c \
		$(BOOM)/ComplexNum.C
#---------------------------------------------------------
$(OBJ)/ConfigFile.o:\
		$(BOOM)/ConfigFile.H \
		$(BOOM)/ConfigFile.C
	$(CC) $(CFLAGS) -o $(OBJ)/ConfigFile.o -c \
		$(BOOM)/ConfigFile.C
#---------------------------------------------------------
$(OBJ)/ContingencyTbl.o:\
		$(BOOM)/ContingencyTbl.H \
		$(BOOM)/ContingencyTbl.C
	$(CC) $(CFLAGS) -o $(OBJ)/ContingencyTbl.o -c \
		$(BOOM)/ContingencyTbl.C
#---------------------------------------------------------
$(OBJ)/Correlation.o:\
		$(BOOM)/Correlation.H \
		$(BOOM)/Correlation.C
	$(CC) $(CFLAGS) -o $(OBJ)/Correlation.o -c \
		$(BOOM)/Correlation.C
#---------------------------------------------------------
$(OBJ)/Cypher.o:\
		$(BOOM)/Cypher.H \
		$(BOOM)/Cypher.C
	$(CC) $(CFLAGS) -o $(OBJ)/Cypher.o -c \
		$(BOOM)/Cypher.C
#---------------------------------------------------------
$(OBJ)/Essex.o:\
		$(BOOM)/Essex.H \
		$(BOOM)/Essex.C
	$(CC) $(CFLAGS) -o $(OBJ)/Essex.o -c \
		$(BOOM)/Essex.C
#---------------------------------------------------------
$(OBJ)/DblMatrix.o:\
		$(BOOM)/DblMatrix.H \
		$(BOOM)/DblMatrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/DblMatrix.o -c \
		$(BOOM)/DblMatrix.C
#---------------------------------------------------------
$(OBJ)/DegenerateDnaAlphabet.o:\
		$(BOOM)/DegenerateDnaAlphabet.H \
		$(BOOM)/DegenerateDnaAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/DegenerateDnaAlphabet.o -c \
		$(BOOM)/DegenerateDnaAlphabet.C
#---------------------------------------------------------
$(OBJ)/DnaAlphabetWithGaps.o:\
		$(BOOM)/DnaAlphabetWithGaps.H \
		$(BOOM)/DnaAlphabetWithGaps.C
	$(CC) $(CFLAGS) -o $(OBJ)/DnaAlphabetWithGaps.o -c \
		$(BOOM)/DnaAlphabetWithGaps.C
#---------------------------------------------------------
$(OBJ)/DnaAlphabet.o:\
		$(BOOM)/DnaAlphabet.H \
		$(BOOM)/DnaAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/DnaAlphabet.o -c \
		$(BOOM)/DnaAlphabet.C
#---------------------------------------------------------
$(OBJ)/DnaDashDotAlphabet.o:\
		$(BOOM)/DnaDashDotAlphabet.H \
		$(BOOM)/DnaDashDotAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/DnaDashDotAlphabet.o -c \
		$(BOOM)/DnaDashDotAlphabet.C
#---------------------------------------------------------
$(OBJ)/DnaDashAlphabet.o:\
		$(BOOM)/DnaDashAlphabet.H \
		$(BOOM)/DnaDashAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/DnaDashAlphabet.o -c \
		$(BOOM)/DnaDashAlphabet.C
#---------------------------------------------------------
$(OBJ)/DnaSubstitutionMatrix.o:\
		$(BOOM)/DnaSubstitutionMatrix.H \
		$(BOOM)/DnaSubstitutionMatrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/DnaSubstitutionMatrix.o -c \
		$(BOOM)/DnaSubstitutionMatrix.C
#---------------------------------------------------------
$(OBJ)/EdgeWeights.o:\
		$(BOOM)/EdgeWeights.H \
		$(BOOM)/EdgeWeights.C
	$(CC) $(CFLAGS) -o $(OBJ)/EdgeWeights.o -c \
		$(BOOM)/EdgeWeights.C
#---------------------------------------------------------
$(OBJ)/EdgeWeightsMatrix.o:\
		$(BOOM)/EdgeWeightsMatrix.H \
		$(BOOM)/EdgeWeightsMatrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/EdgeWeightsMatrix.o -c \
		$(BOOM)/EdgeWeightsMatrix.C
#---------------------------------------------------------
$(OBJ)/Entropy.o:\
		$(BOOM)/Entropy.H \
		$(BOOM)/Entropy.C
	$(CC) $(CFLAGS) -o $(OBJ)/Entropy.o -c \
		$(BOOM)/Entropy.C
#---------------------------------------------------------
$(OBJ)/Environment.o:\
		$(BOOM)/Environment.H \
		$(BOOM)/Environment.C
	$(CC) $(CFLAGS) -o $(OBJ)/Environment.o -c \
		$(BOOM)/Environment.C
#---------------------------------------------------------
$(OBJ)/Exceptions.o:\
		$(BOOM)/Exceptions.H \
		$(BOOM)/Exceptions.C
	$(CC) $(CFLAGS) -o $(OBJ)/Exceptions.o -c \
		$(BOOM)/Exceptions.C
#---------------------------------------------------------
$(OBJ)/ExtremeValueDistribution.o:\
		$(BOOM)/ExtremeValueDistribution.H \
		$(BOOM)/ExtremeValueDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/ExtremeValueDistribution.o -c \
		$(BOOM)/ExtremeValueDistribution.C
#---------------------------------------------------------
$(OBJ)/Factorial.o:\
		$(BOOM)/Factorial.H \
		$(BOOM)/Factorial.C
	$(CC) $(CFLAGS) -o $(OBJ)/Factorial.o -c \
		$(BOOM)/Factorial.C
#---------------------------------------------------------
$(OBJ)/FastaReader.o:\
		$(BOOM)/FastaReader.H \
		$(BOOM)/FastaReader.C
	$(CC) $(CFLAGS) -o $(OBJ)/FastaReader.o -c \
		$(BOOM)/FastaReader.C
#---------------------------------------------------------
$(OBJ)/FastaWriter.o:\
		$(BOOM)/FastaWriter.H \
		$(BOOM)/FastaWriter.C
	$(CC) $(CFLAGS) -o $(OBJ)/FastaWriter.o -c \
		$(BOOM)/FastaWriter.C
#---------------------------------------------------------
$(OBJ)/File.o:\
		$(BOOM)/File.H \
		$(BOOM)/File.C
	$(CC) $(CFLAGS) -o $(OBJ)/File.o -c \
		$(BOOM)/File.C
#---------------------------------------------------------
$(OBJ)/FixedSizePriorityQueue.o:\
		$(BOOM)/FixedSizePriorityQueue.H \
		$(BOOM)/FixedSizePriorityQueue.C
	$(CC) $(CFLAGS) -o $(OBJ)/FixedSizePriorityQueue.o -c \
		$(BOOM)/FixedSizePriorityQueue.C
#---------------------------------------------------------
$(OBJ)/GaussJordan.o:\
		$(BOOM)/GaussJordan.H \
		$(BOOM)/GaussJordan.C
	$(CC) $(CFLAGS) -o $(OBJ)/GaussJordan.o -c \
		$(BOOM)/GaussJordan.C
#---------------------------------------------------------
$(OBJ)/ElasVector.o:\
		$(BOOM)/ElasVector.H \
		$(BOOM)/ElasVector.C
	$(CC) $(CFLAGS) -o $(OBJ)/ElasVector.o -c \
		$(BOOM)/ElasVector.C
#---------------------------------------------------------
$(OBJ)/GffExon.o:\
		$(BOOM)/GffExon.H \
		$(BOOM)/GffExon.C
	$(CC) $(CFLAGS) -o $(OBJ)/GffExon.o -c \
		$(BOOM)/GffExon.C
#---------------------------------------------------------
$(OBJ)/GffFeature.o:\
		$(BOOM)/GffFeature.H \
		$(BOOM)/GffFeature.C
	$(CC) $(CFLAGS) -o $(OBJ)/GffFeature.o -c \
		$(BOOM)/GffFeature.C
#---------------------------------------------------------
$(OBJ)/GffReader.o:\
		$(BOOM)/GffReader.H \
		$(BOOM)/GffReader.C
	$(CC) $(CFLAGS) -o $(OBJ)/GffReader.o -c \
		$(BOOM)/GffReader.C
#---------------------------------------------------------
$(OBJ)/GffTranscript.o:\
		$(BOOM)/GffTranscript.H \
		$(BOOM)/GffTranscript.C
	$(CC) $(CFLAGS) -o $(OBJ)/GffTranscript.o -c \
		$(BOOM)/GffTranscript.C
#---------------------------------------------------------
$(OBJ)/Graph.o:\
		$(BOOM)/Graph.H \
		$(BOOM)/Graph.C
	$(CC) $(CFLAGS) -o $(OBJ)/Graph.o -c \
		$(BOOM)/Graph.C
#---------------------------------------------------------
$(OBJ)/HAF.o:\
		$(BOOM)/HAF.H \
		$(BOOM)/HAF.C
	$(CC) $(CFLAGS) -o $(OBJ)/HAF.o -c \
		$(BOOM)/HAF.C
#---------------------------------------------------------
$(OBJ)/Hashpjw.o:\
		$(BOOM)/Hashpjw.H \
		$(BOOM)/Hashpjw.C
	$(CC) $(CFLAGS) -o $(OBJ)/Hashpjw.o -c \
		$(BOOM)/Hashpjw.C
#---------------------------------------------------------
$(OBJ)/HugeFastaReader.o:\
		$(BOOM)/HugeFastaReader.H \
		$(BOOM)/HugeFastaReader.C
	$(CC) $(CFLAGS) -o $(OBJ)/HugeFastaReader.o -c \
		$(BOOM)/HugeFastaReader.C
#---------------------------------------------------------
$(OBJ)/IncidenceGraph.o:\
		$(BOOM)/IncidenceGraph.H \
		$(BOOM)/IncidenceGraph.C
	$(CC) $(CFLAGS) -o $(OBJ)/IncidenceGraph.o -c \
		$(BOOM)/IncidenceGraph.C
#---------------------------------------------------------
$(OBJ)/IndexedFasta.o:\
		$(BOOM)/IndexedFasta.H \
		$(BOOM)/IndexedFasta.C
	$(CC) $(CFLAGS) -o $(OBJ)/IndexedFasta.o -c \
		$(BOOM)/IndexedFasta.C
#---------------------------------------------------------
$(OBJ)/InversionMatrix.o:\
		$(BOOM)/InversionMatrix.H \
		$(BOOM)/InversionMatrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/InversionMatrix.o -c \
		$(BOOM)/InversionMatrix.C
#---------------------------------------------------------
$(OBJ)/Iterator.o:\
		$(BOOM)/Iterator.H \
		$(BOOM)/Iterator.C
	$(CC) $(CFLAGS) -o $(OBJ)/Iterator.o -c \
		$(BOOM)/Iterator.C
#---------------------------------------------------------
$(OBJ)/JointDistr.o:\
		$(BOOM)/JointDistr.H \
		$(BOOM)/JointDistr.C
	$(CC) $(CFLAGS) -o $(OBJ)/JointDistr.o -c \
		$(BOOM)/JointDistr.C
#---------------------------------------------------------
$(OBJ)/LinRegressor.o:\
		$(BOOM)/LinRegressor.H \
		$(BOOM)/LinRegressor.C
	$(CC) $(CFLAGS) -o $(OBJ)/LinRegressor.o -c \
		$(BOOM)/LinRegressor.C
#---------------------------------------------------------
$(OBJ)/LinearFunc.o:\
		$(BOOM)/LinearFunc.H \
		$(BOOM)/LinearFunc.C
	$(CC) $(CFLAGS) -o $(OBJ)/LinearFunc.o -c \
		$(BOOM)/LinearFunc.C
#---------------------------------------------------------
$(OBJ)/MemStream.o:\
		$(BOOM)/MemStream.H \
		$(BOOM)/MemStream.C
	$(CC) $(CFLAGS) -o $(OBJ)/MemStream.o -c \
		$(BOOM)/MemStream.C
#---------------------------------------------------------
$(OBJ)/MemoryProfiler.o:\
		$(BOOM)/MemoryProfiler.H \
		$(BOOM)/MemoryProfiler.C
	$(CC) $(CFLAGS) -o $(OBJ)/MemoryProfiler.o -c \
		$(BOOM)/MemoryProfiler.C
#---------------------------------------------------------
$(OBJ)/MinimalSpanningTree.o:\
		$(BOOM)/MinimalSpanningTree.H \
		$(BOOM)/MinimalSpanningTree.C
	$(CC) $(CFLAGS) -o $(OBJ)/MinimalSpanningTree.o -c \
		$(BOOM)/MinimalSpanningTree.C
#---------------------------------------------------------
$(OBJ)/MultRegress.o:\
		$(BOOM)/MultRegress.H \
		$(BOOM)/MultRegress.C
	$(CC) $(CFLAGS) -o $(OBJ)/MultRegress.o -c \
		$(BOOM)/MultRegress.C
#---------------------------------------------------------
$(OBJ)/MultSeqAlignment.o:\
		$(BOOM)/MultSeqAlignment.H \
		$(BOOM)/MultSeqAlignment.C
	$(CC) $(CFLAGS) -o $(OBJ)/MultSeqAlignment.o -c \
		$(BOOM)/MultSeqAlignment.C
#---------------------------------------------------------
$(OBJ)/MultiAlignment.o:\
		$(BOOM)/MultiAlignment.H \
		$(BOOM)/MultiAlignment.C
	$(CC) $(CFLAGS) -o $(OBJ)/MultiAlignment.o -c \
		$(BOOM)/MultiAlignment.C
#---------------------------------------------------------
$(OBJ)/MultiHAF.o:\
		$(BOOM)/MultiHAF.H \
		$(BOOM)/MultiHAF.C
	$(CC) $(CFLAGS) -o $(OBJ)/MultiHAF.o -c \
		$(BOOM)/MultiHAF.C
#---------------------------------------------------------
$(OBJ)/MutInfo.o:\
		$(BOOM)/MutInfo.H \
		$(BOOM)/MutInfo.C
	$(CC) $(CFLAGS) -o $(OBJ)/MutInfo.o -c \
		$(BOOM)/MutInfo.C
#---------------------------------------------------------
$(OBJ)/Semaphore.o:\
		$(BOOM)/Semaphore.H \
		$(BOOM)/Semaphore.C
	$(CC) $(CFLAGS) -o $(OBJ)/Semaphore.o -c \
		$(BOOM)/Semaphore.C
#---------------------------------------------------------
$(OBJ)/Mutex.o:\
		$(BOOM)/Mutex.H \
		$(BOOM)/Mutex.C
	$(CC) $(CFLAGS) -o $(OBJ)/Mutex.o -c \
		$(BOOM)/Mutex.C
#---------------------------------------------------------
$(OBJ)/NormalDistribution.o:\
		$(BOOM)/NormalDistribution.H \
		$(BOOM)/NormalDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/NormalDistribution.o -c \
		$(BOOM)/NormalDistribution.C
#---------------------------------------------------------
$(OBJ)/NthOrderStringIterator.o:\
		$(BOOM)/NthOrderStringIterator.H \
		$(BOOM)/NthOrderStringIterator.C
	$(CC) $(CFLAGS) -o $(OBJ)/NthOrderStringIterator.o -c \
		$(BOOM)/NthOrderStringIterator.C
#---------------------------------------------------------
$(OBJ)/Pipe.o:\
		$(BOOM)/Pipe.H \
		$(BOOM)/Pipe.C
	$(CC) $(CFLAGS) -o $(OBJ)/Pipe.o -c \
		$(BOOM)/Pipe.C
#---------------------------------------------------------
$(OBJ)/PoissonDistribution.o:\
		$(BOOM)/PoissonDistribution.H \
		$(BOOM)/PoissonDistribution.C
	$(CC) $(CFLAGS) -o $(OBJ)/PoissonDistribution.o -c \
		$(BOOM)/PoissonDistribution.C
#---------------------------------------------------------
$(OBJ)/Progress.o:\
		$(BOOM)/Progress.H \
		$(BOOM)/Progress.C
	$(CC) $(CFLAGS) -o $(OBJ)/Progress.o -c \
		$(BOOM)/Progress.C
#---------------------------------------------------------
$(OBJ)/ProteinTrans.o:\
		$(BOOM)/ProteinTrans.H \
		$(BOOM)/ProteinTrans.C
	$(CC) $(CFLAGS) -o $(OBJ)/ProteinTrans.o -c \
		$(BOOM)/ProteinTrans.C
#---------------------------------------------------------
$(OBJ)/PureDnaAlphabet.o:\
		$(BOOM)/PureDnaAlphabet.H \
		$(BOOM)/PureDnaAlphabet.C
	$(CC) $(CFLAGS) -o $(OBJ)/PureDnaAlphabet.o -c \
		$(BOOM)/PureDnaAlphabet.C
#---------------------------------------------------------
$(OBJ)/Random.o:\
		$(BOOM)/Random.H \
		$(BOOM)/Random.C
	$(CC) $(CFLAGS) -o $(OBJ)/Random.o -c \
		$(BOOM)/Random.C
#---------------------------------------------------------
$(OBJ)/RealVector.o:\
		$(BOOM)/RealVector.H \
		$(BOOM)/RealVector.C
	$(CC) $(CFLAGS) -o $(OBJ)/RealVector.o -c \
		$(BOOM)/RealVector.C
#---------------------------------------------------------
$(OBJ)/Regex.o:\
		$(BOOM)/Regex.H \
		$(BOOM)/Regex.C
	$(CC) $(CFLAGS) -o $(OBJ)/Regex.o -c \
		$(BOOM)/Regex.C
#---------------------------------------------------------
$(OBJ)/regex.o:\
		$(BOOM)/regex.h \
		$(BOOM)/regex.c
	$(C) $(CFLAGS) -o $(OBJ)/regex.o -c \
		$(BOOM)/regex.c
#---------------------------------------------------------
$(OBJ)/RouletteWheel.o:\
		$(BOOM)/RouletteWheel.H \
		$(BOOM)/RouletteWheel.C
	$(CC) $(CFLAGS) -o $(OBJ)/RouletteWheel.o -c \
		$(BOOM)/RouletteWheel.C
#---------------------------------------------------------
$(OBJ)/Sequence.o:\
		$(BOOM)/Sequence.H \
		$(BOOM)/Sequence.C
	$(CC) $(CFLAGS) -o $(OBJ)/Sequence.o -c \
		$(BOOM)/Sequence.C
#---------------------------------------------------------
$(OBJ)/SequenceEntropy.o:\
		$(BOOM)/SequenceEntropy.H \
		$(BOOM)/SequenceEntropy.C
	$(CC) $(CFLAGS) -o $(OBJ)/SequenceEntropy.o -c \
		$(BOOM)/SequenceEntropy.C
#---------------------------------------------------------
$(OBJ)/SparseGraph.o:\
		$(BOOM)/SparseGraph.H \
		$(BOOM)/SparseGraph.C
	$(CC) $(CFLAGS) -o $(OBJ)/SparseGraph.o -c \
		$(BOOM)/SparseGraph.C
#---------------------------------------------------------
$(OBJ)/Stacktrace.o:\
		$(BOOM)/Stacktrace.H \
		$(BOOM)/Stacktrace.C
	$(CC) $(CFLAGS) -o $(OBJ)/Stacktrace.o -c \
		$(BOOM)/Stacktrace.C
#---------------------------------------------------------
$(OBJ)/StrTokenizer.o:\
		$(BOOM)/StrTokenizer.H \
		$(BOOM)/StrTokenizer.C
	$(CC) $(CFLAGS) -o $(OBJ)/StrTokenizer.o -c \
		$(BOOM)/StrTokenizer.C
#---------------------------------------------------------
$(OBJ)/IndexMap.o:\
		$(BOOM)/IndexMap.H \
		$(BOOM)/IndexMap.C
	$(CC) $(CFLAGS) -o $(OBJ)/IndexMap.o -c \
		$(BOOM)/IndexMap.C
#---------------------------------------------------------
$(OBJ)/String.o:\
		$(BOOM)/String.H \
		$(BOOM)/String.C
	$(CC) $(CFLAGS) -o $(OBJ)/String.o -c \
		$(BOOM)/String.C
#---------------------------------------------------------
$(OBJ)/SummaryStats.o:\
		$(BOOM)/SummaryStats.H \
		$(BOOM)/SummaryStats.C
	$(CC) $(CFLAGS) -o $(OBJ)/SummaryStats.o -c \
		$(BOOM)/SummaryStats.C
#---------------------------------------------------------
$(OBJ)/Symbol.o:\
		$(BOOM)/Symbol.H \
		$(BOOM)/Symbol.C
	$(CC) $(CFLAGS) -o $(OBJ)/Symbol.o -c \
		$(BOOM)/Symbol.C
#---------------------------------------------------------
$(OBJ)/Thread.o:\
		$(BOOM)/Thread.H \
		$(BOOM)/Thread.C
	$(CC) $(CFLAGS) -o $(OBJ)/Thread.o -c \
		$(BOOM)/Thread.C
#---------------------------------------------------------
$(OBJ)/Time.o:\
		$(BOOM)/Time.H \
		$(BOOM)/Time.C
	$(CC) $(CFLAGS) -o $(OBJ)/Time.o -c \
		$(BOOM)/Time.C
#---------------------------------------------------------
$(OBJ)/TwoBitFastaFile.o:\
		$(BOOM)/TwoBitFastaFile.H \
		$(BOOM)/TwoBitFastaFile.C
	$(CC) $(CFLAGS) -o $(OBJ)/TwoBitFastaFile.o -c \
		$(BOOM)/TwoBitFastaFile.C
#---------------------------------------------------------
$(OBJ)/banded-smith-waterman.o:\
		$(BOOM)/banded-smith-waterman.C
	$(CC) $(CFLAGS) -o $(OBJ)/banded-smith-waterman.o -c \
		$(BOOM)/banded-smith-waterman.C
#---------------------------------------------------------
banded-smith-waterman:\
		$(OBJ)/banded-smith-waterman.o
	$(CC) $(LDFLAGS) -o banded-smith-waterman \
		$(OBJ)/banded-smith-waterman.o \
		-lgsl -lm -lgslcblas -L. -lBOOM 
#---------------------------------------------------------
$(OBJ)/cypher.o:\
		$(BOOM)/cypher.H \
		$(BOOM)/cypher.C
	$(CC) $(CFLAGS) -o $(OBJ)/cypher.o -c \
		$(BOOM)/cypher.C
#---------------------------------------------------------
$(OBJ)/get-subst-matrix.o:\
		$(BOOM)/get-subst-matrix.H \
		$(BOOM)/get-subst-matrix.C
	$(CC) $(CFLAGS) -o $(OBJ)/get-subst-matrix.o -c \
		$(BOOM)/get-subst-matrix.C
#---------------------------------------------------------
$(OBJ)/matrix-stats.o:\
		$(BOOM)/matrix-stats.H \
		$(BOOM)/matrix-stats.C
	$(CC) $(CFLAGS) -o $(OBJ)/matrix-stats.o -c \
		$(BOOM)/matrix-stats.C
#---------------------------------------------------------
$(OBJ)/needleman.o:\
		$(BOOM)/Needleman.H \
		$(BOOM)/needleman.C
	$(CC) $(CFLAGS) -o $(OBJ)/needleman.o -c \
		$(BOOM)/needleman.C
#---------------------------------------------------------
needleman: \
		$(OBJ)/needleman.o
	$(CC) $(LDFLAGS) -o needleman \
		$(OBJ)/needleman.o \
		-L. -lBOOM
#---------------------------------------------------------
$(OBJ)/rapid-comparison.o:\
		$(BOOM)/rapid-comparison.H \
		$(BOOM)/rapid-comparison.C
	$(CC) $(CFLAGS) -o $(OBJ)/rapid-comparison.o -c \
		$(BOOM)/rapid-comparison.C
#---------------------------------------------------------
$(OBJ)/refalign.o:\
		$(BOOM)/refalign.H \
		$(BOOM)/refalign.C
	$(CC) $(CFLAGS) -o $(OBJ)/refalign.o -c \
		$(BOOM)/refalign.C
#---------------------------------------------------------
$(OBJ)/smith-waterman.o:\
		$(BOOM)/smith-waterman.H \
		$(BOOM)/smith-waterman.C
	$(CC) $(CFLAGS) -o $(OBJ)/smith-waterman.o -c \
		$(BOOM)/smith-waterman.C
#---------------------------------------------------------
$(OBJ)/test-binary-tree.o:\
		$(BOOM)/test-binary-tree.H \
		$(BOOM)/test-binary-tree.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-binary-tree.o -c \
		$(BOOM)/test-binary-tree.C
#---------------------------------------------------------
$(OBJ)/test-evd.o:\
		$(BOOM)/test-evd.H \
		$(BOOM)/test-evd.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-evd.o -c \
		$(BOOM)/test-evd.C
#---------------------------------------------------------
$(OBJ)/test-factorial.o:\
		$(BOOM)/test-factorial.H \
		$(BOOM)/test-factorial.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-factorial.o -c \
		$(BOOM)/test-factorial.C
#---------------------------------------------------------
$(OBJ)/test-normal.o:\
		$(BOOM)/test-normal.H \
		$(BOOM)/test-normal.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-normal.o -c \
		$(BOOM)/test-normal.C
#---------------------------------------------------------
$(OBJ)/test-poisson.o:\
		$(BOOM)/test-poisson.H \
		$(BOOM)/test-poisson.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-poisson.o -c \
		$(BOOM)/test-poisson.C
#---------------------------------------------------------
$(OBJ)/test-priority-list.o:\
		$(BOOM)/test-priority-list.H \
		$(BOOM)/test-priority-list.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-priority-list.o -c \
		$(BOOM)/test-priority-list.C
#---------------------------------------------------------
$(OBJ)/test-threads.o:\
		$(BOOM)/test-threads.H \
		$(BOOM)/test-threads.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-threads.o -c \
		$(BOOM)/test-threads.C
#---------------------------------------------------------
$(OBJ)/ungapped-aligner.o:\
		$(BOOM)/ungapped-aligner.H \
		$(BOOM)/ungapped-aligner.C
	$(CC) $(CFLAGS) -o $(OBJ)/ungapped-aligner.o -c \
		$(BOOM)/ungapped-aligner.C
#---------------------------------------------------------

#--------------------------------------------------------
$(OBJ)/test-essex.o:\
		test-essex.C
	$(CC) $(CFLAGS) -o $(OBJ)/test-essex.o -c \
		test-essex.C
#---------------------------------------------------------
test-essex: \
		$(OBJ)/test-essex.o
	$(CC) $(LDFLAGS) -o test-essex \
		$(OBJ)/test-essex.o \
		-L. -lBOOM
#--------------------------------------------------------
$(OBJ)/BandedMatrix3D.o:\
		BandedMatrix3D.C\
		BandedMatrix3D.H
	$(CC) $(CFLAGS) -o $(OBJ)/BandedMatrix3D.o -c \
		BandedMatrix3D.C
#--------------------------------------------------------
$(OBJ)/MPI.o:\
		MPI.C
	$(CC) $(CFLAGS) -o $(OBJ)/MPI.o -c \
		MPI.C
#--------------------------------------------------------
$(OBJ)/random.o:\
		random.C
	$(CC) $(CFLAGS) -o $(OBJ)/random.o -c \
		random.C
#---------------------------------------------------------
random: \
		$(OBJ)/random.o
	$(CC) $(LDFLAGS) -o random \
		$(OBJ)/random.o \
		-lgsl -lm -lgslcblas -L. -lBOOM
#--------------------------------------------------------
$(OBJ)/PowerMean.o:\
		PowerMean.C\
		PowerMean.H
	$(CC) $(CFLAGS) -o $(OBJ)/PowerMean.o -c \
		PowerMean.C
#--------------------------------------------------------
$(OBJ)/SumLogProbs.o:\
		SumLogProbs.C\
		SumLogProbs.H
	$(CC) $(CFLAGS) -o $(OBJ)/SumLogProbs.o -c \
		SumLogProbs.C
#---------------------------------------------------------
$(OBJ)/BinaryAlphabet.o:\
		BinaryAlphabet.C\
		BinaryAlphabet.H
	$(CC) $(CFLAGS) -o $(OBJ)/BinaryAlphabet.o -c \
		BinaryAlphabet.C
#---------------------------------------------------------
$(OBJ)/CFasta.o:\
		CFasta.C\
		CFasta.H
	$(CC) $(CFLAGS) -o $(OBJ)/CFasta.o -c \
		CFasta.C
#--------------------------------------------------------
$(OBJ)/GslGaussianMixture.o:\
		$(GSL)/GaussianMixture.C\
		$(GSL)/GaussianMixture.H
	$(CC) $(CFLAGS) -o $(OBJ)/GslGaussianMixture.o -c \
		$(GSL)/GaussianMixture.C
#---------------------------------------------------------

#--------------------------------------------------------
$(OBJ)/FastcReader.o:\
		FastcReader.C\
		FastcReader.H
	$(CC) $(CFLAGS) -o $(OBJ)/FastcReader.o -c \
		FastcReader.C
#---------------------------------------------------------

#--------------------------------------------------------
$(OBJ)/FastbReader.o:\
		FastbReader.C\
		FastbReader.H
	$(CC) $(CFLAGS) -o $(OBJ)/FastbReader.o -c \
		FastbReader.C
#---------------------------------------------------------
$(OBJ)/HigherOrderAlphabet.o:\
		HigherOrderAlphabet.C\
		HigherOrderAlphabet.H
	$(CC) $(CFLAGS) -o $(OBJ)/HigherOrderAlphabet.o -c \
		HigherOrderAlphabet.C
#---------------------------------------------------------

#--------------------------------------------------------
$(OBJ)/Math.o:\
		Math.C\
		Math.H
	$(CC) $(CFLAGS) -o $(OBJ)/Math.o -c \
		Math.C
#---------------------------------------------------------

#--------------------------------------------------------
$(OBJ)/HashTable.o:\
		HashTable.C\
		HashTable.H
	$(CC) $(CFLAGS) -o $(OBJ)/HashTable.o -c \
		HashTable.C
#---------------------------------------------
$(OBJ)/GenericToken.o:\
		GenericToken.C\
		GenericToken.H
	$(CC) $(CFLAGS) -o $(OBJ)/GenericToken.o -c \
		GenericToken.C
#---------------------------------------------------------
$(OBJ)/GenericScanner.o:\
		GenericScanner.C\
		GenericScanner.H
	$(CC) $(CFLAGS) -o $(OBJ)/GenericScanner.o -c \
		GenericScanner.C
#---------------------------------------------------------
$(OBJ)/GenericTokenStream.o:\
		GenericTokenStream.C\
		GenericTokenStream.H
	$(CC) $(CFLAGS) -o $(OBJ)/GenericTokenStream.o -c \
		GenericTokenStream.C
#---------------------------------------------------------
$(OBJ)/SubsetEnumerator.o:\
		SubsetEnumerator.C\
		SubsetEnumerator.H
	$(CC) $(CFLAGS) -o $(OBJ)/SubsetEnumerator.o -c \
		SubsetEnumerator.C
#--------------------------------------------------------
$(OBJ)/CigarString.o:\
		CigarString.C\
		CigarString.H
	$(CC) $(CFLAGS) -o $(OBJ)/CigarString.o -c \
		CigarString.C
#--------------------------------------------------------
$(OBJ)/CigarAlignment.o:\
		CigarAlignment.C\
		CigarAlignment.H
	$(CC) $(CFLAGS) -o $(OBJ)/CigarAlignment.o -c \
		CigarAlignment.C
#---------------------------------------------------------
$(OBJ)/GffGene.o:\
		GffGene.C\
		GffGene.H
	$(CC) $(CFLAGS) -o $(OBJ)/GffGene.o -c \
		GffGene.C
#---------------------------------------------------------
#--------------------------------------------------------
$(OBJ)/GCF.o:\
		GCF.C\
		GCF.H
	$(CC) $(CFLAGS) -o $(OBJ)/GCF.o -c \
		GCF.C
#---------------------------------------------------------

#--------------------------------------------------------
$(OBJ)/Interval.o:\
		Interval.C\
		Interval.H
	$(CC) $(CFLAGS) -o $(OBJ)/Interval.o -c \
		Interval.C
#--------------------------------------------------------
$(OBJ)/CodonIterator.o:\
		CodonIterator.C\
		CodonIterator.H
	$(CC) $(CFLAGS) -o $(OBJ)/CodonIterator.o -c \
		CodonIterator.C
#--------------------------------------------------------
$(OBJ)/map-coord.o:\
		map-coord.C
	$(CC) $(CFLAGS) -o $(OBJ)/map-coord.o -c \
		map-coord.C
#---------------------------------------------------------
map-coord: \
		$(OBJ)/map-coord.o
	$(CC) $(LDFLAGS) -o map-coord \
		$(OBJ)/map-coord.o \
		-lgsl -lm -lgslcblas -L. -lBOOM 
#--------------------------------------------------------
$(OBJ)/HistogramArray.o:\
		HistogramArray.C\
		HistogramArray.H
	$(CC) $(CFLAGS) -o $(OBJ)/HistogramArray.o -c \
		HistogramArray.C
#--------------------------------------------------------
$(OBJ)/Process.o:\
		Process.C\
		Process.H
	$(CC) $(CFLAGS) -o $(OBJ)/Process.o -c \
		Process.C
#---------------------------------------------------------
$(OBJ)/TempFilename.o:\
		TempFilename.C\
		TempFilename.H
	$(CC) $(CFLAGS) -o $(OBJ)/TempFilename.o -c \
		TempFilename.C
#---------------------------------------------------------
$(OBJ)/CombinationIterator.o:\
		CombinationIterator.C\
		CombinationIterator.H
	$(CC) $(CFLAGS) -o $(OBJ)/CombinationIterator.o -c \
		CombinationIterator.C
#--------------------------------------------------------
$(OBJ)/chi-square.o:\
		chi-square.C
	$(CC) $(CFLAGS) -o $(OBJ)/chi-square.o -c \
		chi-square.C
#---------------------------------------------------------
chi-square: \
		$(OBJ)/chi-square.o
	$(CC) $(LDFLAGS) -o chi-square \
		$(OBJ)/chi-square.o \
		-L. -lBOOM
#--------------------------------------------------------
$(OBJ)/FixedSizeQueue.o:\
		FixedSizeQueue.C\
		FixedSizeQueue.H
	$(CC) $(CFLAGS) -o $(OBJ)/FixedSizeQueue.o -c \
		FixedSizeQueue.C
#---------------------------------------------------------
$(OBJ)/VcfReader.o:\
		VcfReader.C\
		VcfReader.H
	$(CC) $(CFLAGS) -o $(OBJ)/VcfReader.o -c \
		VcfReader.C
#---------------------------------------------------------
$(OBJ)/Bed3Record.o:\
		Bed3Record.C\
		Bed3Record.H
	$(CC) $(CFLAGS) -o $(OBJ)/Bed3Record.o -c \
		Bed3Record.C
#---------------------------------------------------------
