#pragma once

#include "fake_proto.h"

namespace sentencepiece {
struct TrainerSpec {
  PB_REPEAT(std::string, input)
  PB_STRING(input_format)
  PB_STRING(model_prefix)

  enum ModelType {
    UNIGRAM = 1,   // Unigram language model with dynamic algorithm
    BPE     = 2,   // Byte Pair Encoding
    WORD    = 3,   // Delimitered by whitespace.
    CHAR    = 4,   // tokenizes into character sequence
  };
  private:
    ModelType model_type_{ModelType::UNIGRAM};
  public:
    const ModelType& model_type() const { return model_type_; }
    void set_model_type(ModelType value) { model_type_ = value; }

  PB_INT32(vocab_size, 8000)
  PB_REPEAT(std::string, accept_language)
  PB_INT32(self_test_sample_size, 0)
  
  ///////////////////////////////////////////////////////////////////
  // Training parameters.
  //
  // Uses characters which cover the corpus with the ratio of `chars_coverage`.
  // This parameter determines the set of basic Alphabet of sentence piece.
  // 1.0 - `chars_coverage` characters are treated as UNK.
  PB_FLOAT(character_coverage, 0.9995)

  // Maximum size of sentences the trainer loads from `input` parameter.
  // Trainer simply loads the `input` files in sequence.
  // It is better to shuffle the input corpus randomly.
  PB_INT32(input_sentence_size, 10000000);

  // Maximum size of sentences to make seed sentence pieces.
  // Extended suffix array is constructed to extract frequent
  // sub-strings from the corpus. This uses 20N working space,
  // where N is the size of corpus.
  PB_INT32(mining_sentence_size, 2000000);

  // Maximum size of sentences to train sentence pieces.
  PB_INT32(training_sentence_size, 10000000);

  // The size of seed sentencepieces.
  // `seed_sentencepiece_size` must be larger than `vocab_size`.
  PB_INT32(seed_sentencepiece_size, 1000000);

  // In every EM sub-iterations, keeps top
  // `shrinking_factor` * `current sentencepieces size` with respect to
  // the loss of the sentence piece. This value should be smaller than 1.0.
  PB_FLOAT(shrinking_factor, 0.75);

  // The maximum sentence length in byte. The sentences with the length
  // larger than `max_sentence_length` is simply ignored.
  // Longer input tends to bring the following risks:
  //  * Overflow during EM training (unigram language model only)
  //  * Performance drop because of O(n log n) cost in BPE.
  PB_INT32(max_sentence_length, 2048);

  // Number of threads in the training.
  PB_INT32(num_threads, 16);

  // Number of EM sub iterations.
  PB_INT32(num_sub_iterations, 2);

  ///////////////////////////////////////////////////////////////////
  // SentencePiece parameters which control the shapes of sentence piece.
  //
  // Maximum length of sentencepiece.
  PB_INT32(max_sentencepiece_length, 16);

  // Uses Unicode script to split sentence pieces.
  // When `split_by_unicode_script` is true, we do not allow sentence piece to
  // include multiple Unicode scripts, e.g. "F1" is not a valid piece.
  // Exception: CJ characters (Hiragana/Katakana/Han) are all handled
  // as one script type, since Japanese word can consist of multiple scripts.
  // This exception is always applied regardless of the accept-language
  // parameter.
  PB_BOOL(split_by_unicode_script, true);

  // When `split_by_number` is true, put a boundary between number and non-number
  // transition. If we want to treat "F1" is one token, set this flag to be false.
  PB_BOOL(split_by_number, true);

  // Use a white space to split sentence pieces.
  // When `split_by_whitespace` is false, we may have the piece containing
  // a white space in the middle. e.g., "in_the".
  PB_BOOL(split_by_whitespace, true);

  ///////////////////////////////////////////////////////////////////
  // Vocabulary management
  //
  // Defines control symbols used as an indicator to
  // change the behavior of the decoder. <s> and </s> are pre-defined.
  // We can use this field to encode various meta information,
  // including language indicator in multilingual model.
  // These symbols are not visible to users, but visible to
  // the decoder. Note that when the input sentence contains control symbols,
  // they are not treated as one token, but segmented into normal pieces.
  // Control symbols must be inserted independently from the segmentation.
  PB_REPEAT(std::string, control_symbols);

  // Defines user defined symbols.
  // These symbols are added with extremely high score
  // so they are always treated as one unique symbol in any context.
  // Typical usage of user_defined_symbols is placeholder for named entities.
  PB_REPEAT(std::string, user_defined_symbols);

  // `vocab_size` is treated as hard limit. Crash if
  // the model can not produce the vocab of size `vocab_size`,
  // When `hard_vocab_limit` is false, vocab_size is treated
  // as soft limit. Note that when model_type=char,
  // always assumes hard_vocab_limit = false.
  PB_BOOL(hard_vocab_limit, true);

  // use all symbols for vocab extraction. This flag is valid
  // if model type is either CHAR or WORD
  PB_BOOL(use_all_vocab, true);

  ///////////////////////////////////////////////////////////////////
  // Reserved special meta tokens.
  // * -1 is not used.
  // * unk_id must not be -1.
  // Id must starts with 0 and be contigous.
  PB_INT32(unk_id, 0);    // <unk>
  PB_INT32(bos_id, 1);    // <s>
  PB_INT32(eos_id, 2);    // </s>
  PB_INT32(pad_id, -1);   // <pad> (padding)

  // Encodes <unk> into U+2047 (DOUBLE QUESTION MARK),
  // since this character can be useful both for user and
  // developer. We can easily figure out that <unk> is emitted.
  PB_STRING_DEFAULT(unk_surface, " \xE2\x81\x87 ");

  void Clear() {}
  std::string Utf8DebugString() { return "Dummy"; }
};

// NormalizerSpec encodes a various parameters for string normalizaiton
struct NormalizerSpec {
  // name of normalization rule.
  PB_STRING(name);

  // Pre-compiled normalization rule created by
  // Builder::GetPrecompiledCharsMap() or Builder::CompileCharsMap() method.
  // Usually this field is set by Builder::GetNormalizerSpec() method.
  PB_STRING(precompiled_charsmap);

  // Adds dummy whitespace at the beginning of text in order to
  // treat "world" in "world" and "hello world" in the same way.
  PB_BOOL(add_dummy_prefix, true);

  // Removes leading, trailing, and duplicate internal whitespace.
  PB_BOOL(remove_extra_whitespaces, true);

  // Replaces whitespace with meta symbol.
  // This field must be true to train sentence piece model.
  PB_BOOL(escape_whitespaces, true);

  // Custom normalization rule file in TSV format.
  // https://github.com/google/sentencepiece/blob/master/doc/normalization.md
  // This field is only used in SentencePieceTrainer::Train() method, which
  // compiles the rule into the binary rule stored in `precompiled_charsmap`.
  PB_STRING(normalization_rule_tsv);

  void Clear() {}
  std::string Utf8DebugString() { return "Dummy"; }
};

// Proto to store samples for self-testing.
struct SelfTestData {
  struct Sample {
    PB_STRING(input);
    PB_STRING(expected);
  };
  PB_REPEAT(Sample, samples);

  void Clear() {}
  std::string Utf8DebugString() { return "Dummy"; }
};

// ModelProto stores model parameters.
// SentencePieceProcessor is supposed to be self-contained.
// All settings/parameters which may change the behavior must be encoded
// in ModelProto.
struct ModelProto {
  struct SentencePiece {

    enum Type {
      NORMAL       = 1,  // normal symbol
      UNKNOWN      = 2,  // unknown symbol. only <unk> for now.
      CONTROL      = 3,  // control symbols. </s>, <s>, <2ja> etc.
      USER_DEFINED = 4,  // user defined symbols.
                         // Typical usage of USER_DEFINED symbol
                         // is placeholder.
      UNUSED       = 5  // this piece is not used.
    };
  private:
    Type type_{Type::NORMAL};
  public:
    const Type& type() const { return type_; }
    void set_type(Type value) { type_ = value; }

    PB_STRING(piece);   // piece must not be empty.
    PB_FLOAT(score, 0);
    
  void Clear() {}
  std::string Utf8DebugString() { return "Dummy"; }
  };

  // Sentence pieces with scores.
  PB_REPEAT(SentencePiece, pieces);

  // Spec used to generate this model file.
  PB_MEMBER(TrainerSpec, trainer_spec);

  // Spec for text normalization.
  PB_MEMBER(NormalizerSpec, normalizer_spec);

  // Stores sample input and its expected segmentation to verify the model.
  PB_MEMBER(SelfTestData, self_test_data);

  void Clear() {}
  std::string Utf8DebugString() { return "Dummy"; }

  bool ParseFromArray(const char*, size_t) { return true; }
  std::string SerializeAsString() { return ""; }
};

}