#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <fstream>

struct Size
{
	Size() = default;
	Size(int w, int h) : width(w), height(h) {}
	int width = 0;
	int height = 0;
};

class IImage
{
public:
	virtual void SetPixel(char color) = 0;
	virtual char GetPixel()const = 0;
	virtual Size GetSize()const = 0;
	virtual void Draw() const = 0;
	virtual ~IImage() = default;
};

class CImage : public IImage
{
public:
	CImage(const std::vector<uint8_t>& /*data*/)
	{
		// Декодируем изображение
	}

	void SetPixel(char color) override
	{

	};

	char GetPixel()const override
	{

	};

	void Draw() const override
	{
		std::cout << "Drawing an image\n";
	}

	Size GetSize() const override
	{
		return m_size;
	}
private:
	Size m_size;
	// Прочие данные
};

// Выполняет загрузку URL-а. Уведомляет об окончании загрузки, вызывая callback
void LoadImageFromURL(const std::string& url,
	const std::function<void(std::unique_ptr<IImage>&&)>& callback)
{
	std::cout << "Loading image from URL " << url << std::endl;
	std::ifstream strm(url, std::ios_base::binary);
	if (strm)
	{
		strm.seekg(0, std::ios_base::end);
		auto size = strm.tellg();
		std::vector<uint8_t> buf(static_cast<size_t>(size));
		strm.seekg(0, std::ios_base::beg);
		if (size > 0 && strm.read(reinterpret_cast<char*>(buf.data()), size))
		{
			callback(std::make_unique<CImage>(buf));
			return;
		}
	}
	callback(nullptr);
}

class CImageProxy : public IImage
{
	struct Impl : std::enable_shared_from_this<Impl>
	{
		Impl(const Size& size, const std::string& url)
			: m_size(size)
			, m_url(url)
		{

		}
		Size GetSize() const
		{
			if (m_image)
			{
				// предполагаем, что размеры одинаковые
				return m_image->GetSize();
			}
			return m_size;
		}

		void Draw() const
		{
			if (!m_image && !m_isLoading && !m_loadingError)
			{
				m_isLoading = true;

				std::weak_ptr<const Impl> weakSelf = shared_from_this();
				LoadImageFromURL(m_url, [=](auto&& image) {
					if (auto strongSelf = weakSelf.lock())
					{
						m_isLoading = false;
						m_image = move(image);
						m_loadingError = !m_image;
						if (m_loadingError)
						{
							std::cout << "Image loading has failed\n";
						}
						else
						{
							std::cout << "Image has been loaded successfully\n";
						}
					}
					else
					{
						// объект уже разрушен, просто игнорируем полученное изображение
						// Использовать данные нельзя
					}
					});
			}

			if (m_image)
			{
				m_image->Draw();
			}
			else if (m_isLoading)
			{
				std::cout << "Drawing a loading indicator\n";
				// Рисуем индикатор загрузки
			}
			else if (m_loadingError)
			{
				std::cout << "Drawing 'broken image' icon\n";
				// Рисуем "сломанное" изображение
			}
		}
	private:
		std::string m_url;
		Size m_size;
		mutable bool m_loadingError = false;
		mutable bool m_isLoading = false;
		mutable std::unique_ptr<IImage> m_image;
	};
public:
	CImageProxy(const Size& size, const std::string& url)
		: m_impl(std::make_shared<Impl>(size, url))
	{
	}

	Size GetSize() const override
	{
		return m_impl->GetSize();
	}

	void Draw() const override
	{
		m_impl->Draw();
	}
private:
	std::shared_ptr<Impl> m_impl;
};

void WorkWithImage(IImage& image)
{
	auto size = image.GetSize();
	std::cout << "Image size:" << size.width << "x" << size.height << std::endl;
	image.Draw();
}

int main()
{
	CImageProxy img({ 30, 30 }, "img1.png");
	WorkWithImage(img);
	return 0;
}