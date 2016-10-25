#ifndef OCRMANAGER_H
#define OCRMANAGER_H

class OcrEngine;

class OcrManager
{
public:
    static OcrManager& instance();   

    void setEngine(OcrEngine * engine);
    bool isActive() const;

    OcrManager(OcrManager const&) = delete;
    void operator=(OcrManager const&) = delete;
protected:
     OcrManager();

private:
    OcrEngine * m_engine;
};

#endif 