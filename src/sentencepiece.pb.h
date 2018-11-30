#pragma once

#include "fake_proto.h"

namespace sentencepiece {

struct SentencePieceText {
  struct SentencePiece {
    PB_STRING(piece)
    PB_UINT32(id, 0)

    PB_STRING(surface)
    PB_UINT32(begin, 0)
    PB_UINT32(end, 0)

    void Clear() { piece_.clear(); id_ = 0; surface_.clear(); begin_ = 0; end_ = 0; }
  };

  PB_REPEAT(SentencePiece, pieces)
  PB_STRING(text)
  PB_FLOAT(score, 0.f)

  void Clear() { text_.clear(); pieces_.clear(); score_ = 0; }
  std::string Utf8DebugString() { return text(); }
};

struct NBestSentencePieceText {
  PB_REPEAT(SentencePieceText, nbests)

  void Clear() { nbests_.clear(); }
  std::string Utf8DebugString() { return "Dummy"; }
};

}
