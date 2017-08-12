#pragma once

/** 
 * @file Any.hh
 * @date 2017.08.12
 * @author kmc7468
 * @brief Any Ŭ������ �����մϴ�.
 */

#include <memory>
#include <stdexcept>
#include <typeinfo>

namespace Dlink
{
	/**
	 * @brief Ÿ�Կ� ��� ���� ���� ������ �� �ִ� ��ü�Դϴ�.
	 * @details boost.any�� �۵� ����� ä���߽��ϴ�. �� Ŭ������ �ٸ� ������ ��ӹ��� �� �����ϴ�.
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
		 * @brief �� Any �ν��Ͻ��� ����ϴ�.
		 * @param value ������ ���Դϴ�.
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
		 * @brief ���� Any �ν��Ͻ��� ����� ���� �����ɴϴ�.
		 * @param Ty_ ���ø� �Ű� �����Դϴ�. ����� ���� Ÿ���Դϴ�.
		 * @return ���� Any �ν��Ͻ��� ����� ���Դϴ�.
		 * @exception std::bad_cast ���� �ν��Ͻ��� ����� ���� Ÿ�԰� ���ø� �Ű� ���� Ty_�� Ÿ���� �ٸ��ϴ�.
		 * @exception std::runtime_error ���� �ν��Ͻ��� ��� �ֽ��ϴ�.
		 */
		template<typename Ty_>
		const Ty_& get() const
		{
			if (typeid(Ty_) == data_->type())
			{
				if (is_empty())
					throw std::runtime_error("���� Any �ν��Ͻ��� ��� �ֽ��ϴ�.");
				return reinterpret_cast<const RealData_<Ty_>*>(data_.get())->data;
			}
			throw std::bad_cast();
		}
		/**
		* @brief ���� Any �ν��Ͻ��� ����� ���� �����ɴϴ�.
		* @param Ty_ ���ø� �Ű� �����Դϴ�. ����� ���� Ÿ���Դϴ�.
		* @return ���� Any �ν��Ͻ��� ����� ���Դϴ�.
		* @exception std::bad_cast ���� �ν��Ͻ��� ����� ���� Ÿ�԰� ���ø� �Ű� ���� Ty_�� Ÿ���� �ٸ��ϴ�.
		* @exception std::runtime_error ���� �ν��Ͻ��� ��� �ֽ��ϴ�.
		*/
		template<typename Ty_>
		Ty_& get()
		{
			if (typeid(Ty_) == data_->type())
			{
				if (is_empty())
					throw std::runtime_error("���� Any �ν��Ͻ��� ��� �ֽ��ϴ�.");
				return reinterpret_cast<RealData_<Ty_>*>(data_.get())->data;
			}
			throw std::bad_cast();
		}

	private:
		std::unique_ptr<Data_> data_;
	};
}