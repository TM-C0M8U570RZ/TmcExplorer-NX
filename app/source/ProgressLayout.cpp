#include "ProgressLayout.hpp"
#include "BrowserLayout.hpp"
#include "MainApplication.hpp"
#include <memory>

namespace tmc {
namespace ExplorerNX {

void progressBarWorker(void* arg)
{
    ThreadContext* ctx = static_cast<ThreadContext*>(arg);
    switch (ctx->pm)
    {
        case ProgressMode::MOVE:
            for (u64 i = 0; i < ctx->paths.size(); i++)
            {
                moveFiles(ctx->paths[i], ctx->currentDirectory / ctx->paths[i].filename(), ctx->fileCountProgress, ctx->bytesProgress);
            }
            break;
        case ProgressMode::COPY:
            for (u64 i = 0; i < ctx->paths.size(); i++)
            {
                copyFiles(ctx->paths[i], ctx->currentDirectory / ctx->paths[i].filename(), ctx->fileCountProgress, ctx->bytesProgress);
            }
            break;
        default:
            for (u64 i = 0; i < ctx->paths.size(); i++)
            {
                deleteFiles(ctx->paths[i], ctx->fileCountProgress, ctx->bytesProgress);
            }
    }
}

ProgressLayout::ProgressLayout(MainApplication* app, const std::filesystem::path& currentDirectory, const std::vector<std::filesystem::path> deleteList)
{
    this->app = app;
    this->currentDirectory = currentDirectory;
    this->deleteList = deleteList;
    this->background = pu::ui::elm::Rectangle::New(0, 50, 1920, 1030, pu::ui::Color(127, 127, 255, 255));
    this->progress = pu::ui::elm::ProgressBar::New(10, 515, 1700, 50, 100.0);
    this->progressText = pu::ui::elm::TextBlock::New(1720, 515, "0.00%");
    this->fileCountProgress = 0;
    this->fileCountTotal = 0;
    this->bytesProgress = 0;
    this->bytesTotal = 0;
    this->text = pu::ui::elm::TextBlock::New(10, 10, "");
    Clipboard clip = app->getClipboard();
    if (!deleteList.empty())
    {
        this->pm = ProgressMode::DELETE;
        for (u64 i = 0; i < deleteList.size(); i++)
        {
            this->fileCountTotal += enumerateFiles(deleteList[i]);
            this->bytesTotal += enumerateBytes(deleteList[i]);
        }
        text->SetText("Deleting file 1 of " + std::to_string(fileCountTotal) + " | 0B / " + beautifyByteCount(bytesTotal));
    }
    else if (clip.moving)
    {
        this->pm = ProgressMode::MOVE;
        for (u64 i = 0; i < clip.paths.size(); i++)
        {
            this->fileCountTotal += enumerateFiles(clip.paths[i]);
            this->bytesTotal += enumerateBytes(clip.paths[i]);
        }
        text->SetText("Moving file 1 of " + std::to_string(fileCountTotal) + " | 0B / " + beautifyByteCount(bytesTotal));
    }
    else
    {
        this->pm = ProgressMode::COPY;
        for (u64 i = 0; i < clip.paths.size(); i++)
        {
            this->fileCountTotal += enumerateFiles(clip.paths[i]);
            this->bytesTotal += enumerateBytes(clip.paths[i]);
        }
        text->SetText("Copying file 1 of " + std::to_string(fileCountTotal) + " | 0B / " + beautifyByteCount(bytesTotal));
    }
    ctx = new ThreadContext{
        .pm = this->pm,
        .currentDirectory = this->currentDirectory,
        .paths = ((this->pm == ProgressMode::DELETE) ? deleteList : (clip.paths)),
        .fileCountProgress = &fileCountProgress,
        .bytesProgress = &bytesProgress
    };

    Result rc = threadCreate(&workerThread, progressBarWorker, ctx, nullptr, 0x4000, 0x2B, -2);
    if (R_SUCCEEDED(rc))
    {
        threadStart(&workerThread);
        this->AddRenderCallback([this] {
            u64 currentProgressFiles = fileCountProgress;
            u64 currentProgressBytes = bytesProgress;
            switch (this->pm)
            {
                case ProgressMode::MOVE:
                    text->SetText("Moving file " + std::to_string(currentProgressFiles + 1) + " of " + std::to_string(fileCountTotal) + " | " + beautifyByteCount(currentProgressBytes) + " / " + beautifyByteCount(bytesTotal));
                    break;
                case ProgressMode::COPY:
                    text->SetText("Copying file " + std::to_string(currentProgressFiles + 1) + " of " + std::to_string(fileCountTotal) + " | " + beautifyByteCount(currentProgressBytes) + " / " + beautifyByteCount(bytesTotal));
                    break;
                default:
                    text->SetText("Deleting file " + std::to_string(currentProgressFiles + 1) + " of " + std::to_string(fileCountTotal) + " | " + beautifyByteCount(currentProgressBytes) + " / " + beautifyByteCount(bytesTotal));
            }
            if (bytesTotal != 0)
                progress->SetProgress(double(currentProgressBytes) / bytesTotal * 100.0);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << (double(currentProgressBytes) / bytesTotal * 100.0);
            ss << "%";
            progressText->SetText(ss.str());
            if (currentProgressBytes == bytesTotal)
            {
                progress->FillProgress();
                progressText->SetText("100.00%");
                BrowserLayout::Ref prev = BrowserLayout::New(this->app, this->currentDirectory);
                this->app->LoadLayout(prev);
            }
        });
    }
    this->Add(this->background);
    this->Add(this->text);
    this->Add(this->progress);
    this->Add(this->progressText);
}

ProgressLayout::~ProgressLayout()
{
    threadWaitForExit(&workerThread);
    threadClose(&workerThread);
    delete ctx;
    this->app->clearClipboard();
}

}
}
