/*
 * Copyright (c) 2012-2024 Meltytech, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ENCODEDOCK_H  // 如果没有定义 ENCODEDOCK_H 宏（防止头文件重复包含）
#define ENCODEDOCK_H  // 定义 ENCODEDOCK_H 宏
#include "settings.h"  // 包含自定义的设置头文件
#include <MltProperties.h>  // 包含 MLT 多媒体框架的属性类头文件
#include <QDockWidget>      // 包含 Qt 的停靠窗口部件类头文件
#include <QDomElement>      // 包含 Qt 的 DOM 元素类头文件（用于 XML 操作）
#include <QSortFilterProxyModel>  // 包含 Qt 的排序过滤代理模型类头文件
#include <QStandardItemModel>     // 包含 Qt 的标准项模型类头文件
#include <QStringList>            // 包含 Qt 的字符串列表类头文件

class QTreeWidgetItem;  // 前向声明 Qt 的树形部件项类
class QTemporaryFile;   // 前向声明 Qt 的临时文件类
namespace Ui {  // 声明 Ui 命名空间（由 Qt Designer 生成）
class EncodeDock;  // 前向声明 EncodeDock 的 UI 类
}
class AbstractJob;  // 前向声明抽象任务类
class MeltJob;      // 前向声明 Melt 任务类
namespace Mlt {  // 声明 MLT 命名空间
class Service;   // 前向声明 MLT 服务类
class Producer;  // 前向声明 MLT 生产者类（多媒体源）
class Filter;    // 前向声明 MLT 过滤器类
} // namespace Mlt

class PresetsProxyModel : public QSortFilterProxyModel  // 预设代理模型类，继承自排序过滤代理模型
{
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;  // 重写行过滤接受方法
};

class EncodeDock : public QDockWidget  // 编码停靠窗口类，继承自停靠窗口部件
{
    Q_OBJECT  // Qt 的元对象宏，用于支持信号和槽机制

public:
    explicit EncodeDock(QWidget *parent = 0);  // 显式构造函数，parent 为父部件指针，默认为空
    ~EncodeDock();  // 析构函数

    void loadPresetFromProperties(Mlt::Properties &);  // 从 MLT 属性加载预设
    bool isExportInProgress() const;  // 检查是否正在导出（编码进行中）

signals:
    void captureStateChanged(bool);  // 信号：捕获状态改变
    void createOrEditFilterOnOutput(Mlt::Filter *, const QStringList & = {});  // 信号：在输出上创建或编辑过滤器

public slots:  // 公共槽函数（可在外部连接）
    void onAudioChannelsChanged();  // 音频通道数改变时的槽函数
    void onProducerOpened();        // 生产者（多媒体源）打开时的槽函数
    void onProfileChanged();        // 配置文件改变时的槽函数
    void onReframeChanged();        // 重帧设置改变时的槽函数
    void on_hwencodeButton_clicked();  // 硬件编码按钮点击的槽函数
    bool detectHardwareEncoders();     // 检测硬件编码器

private slots:  // 私有槽函数（内部使用）
    void on_presetsTree_clicked(const QModelIndex &index);      // 预设树点击槽函数
    void on_presetsTree_activated(const QModelIndex &index);    // 预设树激活（如回车）槽函数

    void on_encodeButton_clicked();      // 编码按钮点击槽函数
    void on_streamButton_clicked();      // 流媒体按钮点击槽函数

    void on_addPresetButton_clicked();   // 添加预设按钮点击槽函数
    void on_removePresetButton_clicked(); // 移除预设按钮点击槽函数

    void onFinished(AbstractJob *, bool isSuccess);  // 任务完成时的槽函数

    void on_stopCaptureButton_clicked();  // 停止捕获按钮点击槽函数

    void on_videoRateControlCombo_activated(int index);   // 视频码率控制下拉框激活槽函数
    void on_audioRateControlCombo_activated(int index);   // 音频码率控制下拉框激活槽函数

    void on_scanModeCombo_currentIndexChanged(int index);  // 扫描模式下拉框当前索引改变槽函数

    void on_presetsSearch_textChanged(const QString &search);  // 预设搜索文本框内容改变槽函数

    void on_resetButton_clicked();  // 重置按钮点击槽函数

    void openCaptureFile();  // 打开捕获文件槽函数

    void on_formatCombo_currentIndexChanged(int index);  // 格式下拉框当前索引改变槽函数

    void on_videoBufferDurationChanged();  // 视频缓冲区时长改变槽函数

    void on_gopSpinner_valueChanged(int value);  // GOP（图像组）微调框值改变槽函数

    void on_fromCombo_currentIndexChanged(int index);  // “来源”下拉框当前索引改变槽函数

    void on_videoCodecCombo_currentIndexChanged(int index);  // 视频编码器下拉框当前索引改变槽函数
    void on_audioCodecCombo_currentIndexChanged(int index);  // 音频编码器下拉框当前索引改变槽函数

    void setAudioChannels(int channels);  // 设置音频通道数槽函数

    void on_widthSpinner_editingFinished();  // 宽度微调框编辑完成槽函数
    void on_heightSpinner_editingFinished(); // 高度微调框编辑完成槽函数

    void on_advancedButton_clicked(bool checked);  // 高级按钮点击槽函数（带勾选状态）
    void on_hwencodeCheckBox_clicked(bool checked); // 硬件编码复选框点击槽函数
    void on_advancedCheckBox_clicked(bool checked); // 高级复选框点击槽函数

    void on_fpsSpinner_editingFinished();  // 帧率微调框编辑完成槽函数
    void on_fpsComboBox_activated(int arg1); // 帧率下拉框激活槽函数

    void on_videoQualitySpinner_valueChanged(int vq);  // 视频质量微调框值改变槽函数
    void on_audioQualitySpinner_valueChanged(int aq);  // 音频质量微调框值改变槽函数

    void on_parallelCheckbox_clicked(bool checked);  // 并行编码复选框点击槽函数

    void on_resolutionComboBox_activated(int arg1);  // 分辨率下拉框激活槽函数

    void on_reframeButton_clicked();  // 重帧按钮点击槽函数

    void on_aspectNumSpinner_valueChanged(int value);  // 宽高比分子微调框值改变槽函数
    void on_aspectDenSpinner_valueChanged(int value);  // 宽高比分母微调框值改变槽函数

private:
    enum {  // 枚举：码率控制类型
        RateControlAverage = 0,  // 平均码率
        RateControlConstant,     // 恒定码率
        RateControlQuality,      // 质量模式（如 CRF）
        RateControlConstrained   // 受限码率（如 VBV）
    };
    enum {  // 枚举：音频通道数
        AudioChannels1 = 0,  // 单声道
        AudioChannels2,      // 立体声
        AudioChannels4,      // 4 声道
        AudioChannels6,      // 6 声道（如 5.1）
    };
    Ui::EncodeDock *ui;  // 指向 UI 类的指针（由 Qt Designer 生成）
    Mlt::Properties *m_presets;  // MLT 属性指针，用于存储预设
    QScopedPointer<MeltJob> m_immediateJob;  // 智能指针：当前立即执行的任务（如分析）
    QString m_extension;  // 文件扩展名字符串
    Mlt::Properties *m_profiles;  // MLT 属性指针，用于存储编码配置文件
    PresetsProxyModel m_presetsModel;  // 预设代理模型实例
    QStringList m_outputFilenames;  // 输出文件名列表
    bool m_isDefaultSettings;  // 是否为默认设置的标志
    double m_fps;  // 帧率值
    QStringList m_intraOnlyCodecs;  // 仅帧内编码的编码器列表（如 ProRes）
    QStringList m_losslessVideoCodecs;  // 无损视频编码器列表
    QStringList m_losslessAudioCodecs;  // 无损音频编码器列表

    void loadPresets();  // 加载预设
    Mlt::Properties *collectProperties(int realtime, bool includeProfile = false);  // 收集当前 UI 设置到 MLT 属性
    void collectProperties(QDomElement &node, int realtime);  // 收集属性到 XML DOM 元素
    void setSubtitleProperties(QDomElement &node, Mlt::Producer *service);  // 设置字幕属性到 DOM 元素
    QPoint addConsumerElement(  // 添加消费者元素到 XML DOM，返回点坐标（可能用于定位）
        Mlt::Producer *service, QDomDocument &dom, const QString &target, int realtime, int pass);
    MeltJob *convertReframe(Mlt::Producer *service,  // 创建重帧转换任务
                            QTemporaryFile *tmp,
                            const QString &target,
                            int realtime,
                            int pass,
                            const QThread::Priority priority);
    MeltJob *createMeltJob(Mlt::Producer *service,  // 创建 Melt 任务
                           const QString &target,
                           int realtime,
                           int pass = 0,
                           const QThread::Priority priority = Settings.jobPriority());
    void runMelt(const QString &target, int realtime = -1);  // 运行 Melt 进行编码
    void enqueueAnalysis();  // 将分析任务加入队列（如用于多遍编码）
    void enqueueMelt(const QStringList &targets, int realtime);  // 将 Melt 任务加入队列
    void encode(const QString &target);  // 执行编码
    void resetOptions();  // 重置选项到默认
    Mlt::Producer *fromProducer(bool usePlaylistBin = false) const;  // 获取当前生产者（源）
    static void filterCodecParams(const QString &vcodec, QStringList &other);  // 根据编码器过滤参数
    void onVideoCodecComboChanged(int index, bool ignorePreset = false, bool resetBframes = true);  // 视频编码器改变处理
    bool checkForMissingFiles();  // 检查缺失文件（如多片段源）
    QString &defaultFormatExtension();  // 获取默认格式扩展名
    void initSpecialCodecLists();  // 初始化特殊编码器列表（如无损、仅帧内）
    void setReframeEnabled(bool enabled);  // 设置重帧功能启用状态
    void showResampleWarning(const QString &message);  // 显示重采样警告
    void hideResampleWarning(bool hide = true);  // 隐藏重采样警告
    void checkFrameRate();  // 检查帧率（可能用于验证）
    void setResolutionAspectFromProfile();
};// 类定义结束

#endif // ENCODEDOCK_H
