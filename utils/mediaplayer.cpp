#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavresample/avresample.h>
}

#include "mediaplayer.h"
#include "playlistitem.h"
#include "playlistitemmodel.h"
#include "utils/signalbus.h"

MediaPlayer::MediaPlayer(QListView* listView) : QObject() {
  this->listView = listView;

  /* Other signals */

  connect(SignalBus::getInstance(), &SignalBus::startPlaylist, this,
          &MediaPlayer::startPlaylist);

  connect(SignalBus::getInstance(), &SignalBus::stopPlaylist,
          [=]() { emit SignalBus::getInstance()->playlistStopped(); });
}

void MediaPlayer::startPlaylist() {
  if (!this->listView->selectionModel()->hasSelection()) {
    emit SignalBus::getInstance()->playlistStopped();
    return;
  }
  int selectedRow = this->listView->currentIndex().row();

  PlaylistItemModel* model =
      static_cast<PlaylistItemModel*>(this->listView->model());

  PlaylistItem* playlistItem = model->list().at(selectedRow);

  PlaylistItemType itemType =
      static_cast<PlaylistItemType>(playlistItem->getType());

  emit SignalBus::getInstance()->playlistStarted();

  switch (itemType) {
    case TRACK: {
      TrackItem* trackItem = static_cast<TrackItem*>(playlistItem);

      AVFormatContext* pFormatContext = avformat_alloc_context();

      avformat_open_input(&pFormatContext,
                          trackItem->getPath().toUtf8().constData(), nullptr,
                          nullptr);

      std::cerr << "Format: " << pFormatContext->iformat->long_name
                << ", duration: " << pFormatContext->duration
                << ", nb_streams : " << pFormatContext->nb_streams << std::endl;

      avformat_find_stream_info(pFormatContext, nullptr);

      AVCodec* pCodec = nullptr;
      AVCodecParameters* pCodecParameters = nullptr;
      unsigned int audio_stream_index = -1;

      for (unsigned int i = 0; i < pFormatContext->nb_streams; i++) {
        AVCodecParameters* pLocalCodecParameters =
            pFormatContext->streams[i]->codecpar;

        AVCodec* pLocalCodec =
            avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
          audio_stream_index = i;
          pCodec = pLocalCodec;
          pCodecParameters = pLocalCodecParameters;
          fprintf(stderr, "Audio Codec: %d channels, sample rate %d",
                  pLocalCodecParameters->channels,
                  pLocalCodecParameters->sample_rate);
        }
        fprintf(stderr, "\tCodec %s ID %d bit_rate %ld", pLocalCodec->long_name,
                pLocalCodec->id, pCodecParameters->bit_rate);
        AVCodecContext* pCodecContext = avcodec_alloc_context3(pCodec);
        avcodec_parameters_to_context(pCodecContext, pCodecParameters);
        avcodec_open2(pCodecContext, pCodec, nullptr);
        AVFrame* pFrame = av_frame_alloc();
        AVPacket* pPacket = av_packet_alloc();
      }
      break;
    }
    case CONTROL:
      break;
  }
}
