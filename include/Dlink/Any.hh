#pragma once

/** 
 * @file Any.hh
 * @date 2017.08.12
 * @author kmc7468
 * @brief Any 클래스를 정의합니다.
 */

#include <memory>
#include <stdexcept>
#include <typeinfo>

namespace Dlink
{
	/**
	 * @brief 타입에 상관 없이 값을 저장할 수 있는 객체입니다.
	 * @details boost.any의 작동 방식을 채택했습니다. 이 클래스는 다른 곳에서 상속받을 수 없습니다.
	 */
	class Any final
	{
	private:
		struct Data_
		{
			virtual const std::type_info& type() const = 0;
			virtual std::unique_ptr<Data_> copy() const = 0;
		};
		template<typename Ty_>
		struct RealData_ final : public Data_
		{
			RealData_()
				: data()
			{}
			RealData_(const Ty_& data)
				: data(data)
			{}
			~RealData_() = default;

			virtual const std::type_info& type() const override
			{
				return typeid(Ty_);
			}
			virtual std::unique_ptr<Data_> copy() const override
			{
				return std::make_unique<RealData_<Ty_>>(data);
			}

			Ty_ data;
		};

	public:
		Any();
		/**
		 * @brief 새 Any 인스턴스를 만듭니다.
		 * @param value 저장할 값입니다.
		 */
		template<typename Ty_>
		Any(const Ty_& value)
			: data_(new RealData_<Ty_>(value))
		{}
		Any(const Any& any);
		~Any() = default;

	public:
		Any& operator=(const Any& any);
		bool operator==(const Any& any) const noexcept = delete;
		bool operator!=(const Any& any) const noexcept = delete;

	public:
		bool is_empty() const noexcept;
		const std::type_info& type() const;
		/**
		 * @brief 현재 Any 인스턴스에 저장된 값을 가져옵니다.
		 * @param Ty_ 템플릿 매개 변수입니다. 저장된 값의 타입입니다.
		 * @return 현재 Any 인스턴스에 저장된 값입니다.
		 * @exception std::bad_cast 현재 인스턴스에 저장된 값의 타입과 템플릿 매개 변수 Ty_의 타입이 다릅니다.
		 * @exception std::runtime_error 현재 인스턴스가 비어 있습니다.
		 */
		template<typename Ty_>
		const Ty_& get() const
		{
			if (typeid(Ty_) == data_->type())
			{
				if (is_empty())
					throw std::runtime_error("현재 Any 인스턴스는 비어 있습니다.");
				return reinterpret_cast<const RealData_<Ty_>*>(data_.get())->data;
			}
			throw std::bad_cast();
		}
		/**
		* @brief 현재 Any 인스턴스에 저장된 값을 가져옵니다.
		* @param Ty_ 템플릿 매개 변수입니다. 저장된 값의 타입입니다.
		* @return 현재 Any 인스턴스에 저장된 값입니다.
		* @exception std::bad_cast 현재 인스턴스에 저장된 값의 타입과 템플릿 매개 변수 Ty_의 타입이 다릅니다.
		* @exception std::runtime_error 현재 인스턴스가 비어 있습니다.
		*/
		template<typename Ty_>
		Ty_& get()
		{
			if (typeid(Ty_) == data_->type())
			{
				if (is_empty())
					throw std::runtime_error("현재 Any 인스턴스는 비어 있습니다.");
				return reinterpret_cast<RealData_<Ty_>*>(data_.get())->data;
			}
			throw std::bad_cast();
		}

	private:
		std::unique_ptr<Data_> data_;
	};
}