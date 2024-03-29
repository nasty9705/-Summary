template<typename ValueT>
template<typename T>
inline
	T& Matrix<ValueT>::make_rw(const T& val) const
{
	return const_cast<T&>(val);
}

template<typename ValueT>
Matrix<ValueT>::Matrix(uint row_count, uint col_count) :
	n_rows{ row_count },
	n_cols{ col_count },
	stride{ n_cols },
	pin_row{ 0 },
	pin_col{ 0 },
	_data{}
{
	auto size = n_cols * n_rows;
	if (size)
		_data.reset(new ValueT[size], std::default_delete<ValueT []>());
}

template<typename ValueT>
Matrix<ValueT>::Matrix(std::initializer_list<ValueT> lst) :
	n_rows{ 1 },
	n_cols(lst.size()), // FIXME: narrowing.
	stride{ n_cols },
	pin_row{ 0 },
	pin_col{ 0 },
	_data{}
{
	if (n_cols) {
		_data.reset(new ValueT[n_cols], std::default_delete<ValueT []>());
		std::copy(lst.begin(), lst.end(), _data.get());
	}
}

template<typename ValueT>
Matrix<ValueT> Matrix<ValueT>::deep_copy() const
{
	Matrix<ValueT> tmp(n_rows, n_cols);
	for (uint i = 0; i < n_rows; ++i)
		for (uint j = 0; j < n_cols; ++j)
			tmp(i, j) = (*this)(i, j);
	return tmp;
}

template<typename ValueT>
const Matrix<ValueT> &Matrix<ValueT>::operator = (const Matrix<ValueT> &m)
{
	make_rw(n_rows) = m.n_rows;
	make_rw(n_cols) = m.n_cols;
	make_rw(stride) = m.stride;
	make_rw(pin_row) = m.pin_row;
	make_rw(pin_col) = m.pin_col;
	_data = m._data;
	return *this;
}
template<typename ValueT>
Matrix<ValueT>::Matrix(std::initializer_list < std::initializer_list < ValueT >> lsts) :
	n_rows(lsts.size()), // FIXME: narrowing.
	n_cols{ 0 },
	stride{ n_cols },
	pin_row{ 0 },
	pin_col{ 0 },
	_data{}
{
	// check if no action is needed.
	if (n_rows == 0)
		return;

	// initializing columns count using first row.
	make_rw(n_cols) = lsts.begin()->size();
	make_rw(stride) = n_cols;

	// lambda function to check sublist length.
	// local block to invalidate stack variables after it ends.
	{
		auto local_n_cols = n_cols;
		auto chk_length = [local_n_cols](const std::initializer_list<ValueT> &l) {
			return l.size() == local_n_cols;
		};
		// checking that all row sizes are equal.
		if (! std::all_of(lsts.begin(), lsts.end(), chk_length))
			throw std::string("Initialization rows must have equal length");
	}

	if (n_cols == 0)
		return;

	// allocating matrix memory.
	_data.reset(new ValueT[n_cols * n_rows], std::default_delete<ValueT []>());

	// copying matrix data.
	{
		auto write_ptr = _data.get();
		auto ptr_delta = n_cols;
		auto copier = [&write_ptr, ptr_delta](const std::initializer_list<ValueT> &l) {
			std::copy(l.begin(), l.end(), write_ptr);
			write_ptr += ptr_delta;
		};
		for_each(lsts.begin(), lsts.end(), copier);
	}
}

template<typename ValueT>
Matrix<ValueT>::Matrix(const Matrix &src) :
	n_rows{ src.n_rows },
	n_cols{ src.n_cols },
	stride{ src.stride },
	pin_row{ src.pin_row },
	pin_col{ src.pin_col },
	_data{ src._data }
{
}

template<typename ValueT>
Matrix<ValueT>::Matrix(Matrix && src) :
	n_rows{ src.n_rows },
	n_cols{ src.n_cols },
	stride{ src.stride },
	pin_row{ src.pin_row },
	pin_col{ src.pin_col },
	_data{ src._data }
{
	// resetting state of donor object.
	make_rw(src.n_rows) = 0;
	make_rw(src.n_cols) = 0;
	make_rw(src.stride) = 0;
	make_rw(src.pin_row) = 0;
	make_rw(src.pin_col) = 0;
	src._data.reset();
}


template<typename ValueT>
ValueT &Matrix<ValueT>::operator()(uint row, uint col)
{
	if (row >= n_rows || col >= n_cols)
		throw std::string("Out of bounds");
	row += pin_row;
	col += pin_col;
	return _data.get()[row * stride + col];
}

template<typename ValueT>
const ValueT &Matrix<ValueT>::operator()(uint row, uint col) const
{
	if (row >= n_rows || col >= n_cols)
		throw std::string("Out of bounds");
	row += pin_row;
	col += pin_col;
	return _data.get()[row * stride + col];
}

template<typename ValueT>
Matrix<ValueT>::~Matrix()
{}

template<typename ValueT>
const Matrix<ValueT> Matrix<ValueT>::submatrix(uint prow, uint pcol,
	uint rows, uint cols) const
{
	if (prow + rows > n_rows || pcol + cols > n_cols)
		throw std::string("Out of bounds");
	// copying requested data to submatrix.
	Matrix<ValueT> tmp(*this);
	make_rw(tmp.n_rows) = rows;
	make_rw(tmp.n_cols) = cols;
	make_rw(tmp.pin_row) = pin_row + prow;
	make_rw(tmp.pin_col) = pin_col + pcol;
	return tmp;
}

template<typename ValueT>
template<typename UnaryMatrixOperator>
Matrix<typename std::result_of<UnaryMatrixOperator(Matrix<ValueT>)>::type>
	Matrix<ValueT>::unary_map(const UnaryMatrixOperator &op) const
{
	// Let's typedef return type of function for ease of usage
	typedef typename std::result_of<UnaryMatrixOperator(Matrix<ValueT>)>::type ReturnT;
	if (n_cols * n_rows == 0)
		return Matrix<ReturnT>(0, 0);

	Matrix<ReturnT> tmp(n_rows, n_cols);

	const auto kernel_vert_radius = op.vert_radius;
	const auto kernel_hor_radius = op.hor_radius;

	Matrix<ValueT> extra_image = extra_borders(kernel_vert_radius, kernel_hor_radius);

	for (uint i = 0; i < n_rows; ++i) {
		for (uint j = 0; j < n_cols; ++j) {
			auto neighbourhood = extra_image.submatrix(i, j, 2 * kernel_vert_radius + 1, 2 * kernel_hor_radius + 1);
			tmp(i, j) = op(neighbourhood);
		}
	}
	return tmp;
}


class UnaryMatrixOperator
{
public:
	Matrix<float> kernel;
	uint vert_radius;
	uint hor_radius;
	
	UnaryMatrixOperator(Matrix<float> tmpKernel){
		kernel = tmpKernel;
		vert_radius = kernel.n_rows/2;
		hor_radius = kernel.n_cols/2;
	}

	float operator()(Matrix<float> neighbourhood)
	{
		float tmp = 0;
		for (uint i = 0; i < neighbourhood.n_rows; i++)
			for (uint j = 0;j < neighbourhood.n_cols; j++)
			{
				tmp += kernel(i, j) * neighbourhood(i, j);
			}

		return tmp;
	}
};

template<typename ValueT>
template<typename UnaryMatrixOperator>
Matrix<typename std::result_of<UnaryMatrixOperator(Matrix<ValueT>)>::type>
	Matrix<ValueT>::unary_map(UnaryMatrixOperator &op) const
{
	typedef typename std::result_of<UnaryMatrixOperator(Matrix<ValueT>)>::type ReturnT;
	if (n_cols * n_rows == 0)
		return Matrix<ReturnT>(0, 0);

	Matrix<ReturnT> tmp(n_rows, n_cols);	

	const auto kernel_vert_radius = op.vert_radius;
	const auto kernel_hor_radius = op.hor_radius;

	Matrix<ValueT> extra_image = extra_borders(kernel_vert_radius, kernel_hor_radius);

	for (uint i = 0; i < n_rows; ++i) {
		for (uint j = 0; j < n_cols; ++j) {
			auto neighbourhood = extra_image.submatrix(i, j, 2 * kernel_vert_radius + 1, 2 * kernel_hor_radius + 1);
			tmp(i, j) = op(neighbourhood);
		}
	}
	return tmp;
}

template<typename ValueT>
Matrix<ValueT> Matrix<ValueT>::extra_borders(uint kernel_vert_radius, uint kernel_hor_radius) const
{
	Matrix<ValueT> extra_image = Matrix<ValueT>(n_rows + 2 * kernel_vert_radius, n_cols + 2 * kernel_hor_radius);
	for (uint i = 0; i < n_rows; i++) {
		for (uint j = 0; j < n_cols; j++) {
			extra_image(i + kernel_vert_radius, j + kernel_hor_radius) = (*this)(i, j);
		}
	}
	//top and bottom
	for (uint i = 0; i < kernel_vert_radius; i++) {
		for (uint j = 0; j < n_cols; j++) {
			extra_image(kernel_vert_radius - i - 1, j + kernel_hor_radius) = (*this)(i, j);
			extra_image(n_rows + i + kernel_vert_radius, j + kernel_hor_radius) = (*this)(n_rows - 1 - i, j);
		}
	}
	//left and right
	for (uint i = 0; i < n_rows; i++) {
		for (uint j = 0; j < kernel_hor_radius; j++) {
			extra_image(i + kernel_vert_radius, kernel_hor_radius - j - 1) = (*this)(i, j);
			extra_image(i + kernel_vert_radius, n_cols + kernel_hor_radius + j) = (*this)(i, n_cols - 1 - j);
		}
	}
	//corners
	for (uint i = 0; i < kernel_vert_radius; i++) {
		for (uint j = 0; j < kernel_hor_radius; j++) {
			//top-left
			extra_image(kernel_vert_radius - i - 1, kernel_hor_radius - j - 1) = (*this)(i, j);
			//bottom-right
			extra_image(n_rows + kernel_vert_radius + i, n_cols + kernel_hor_radius + j) = (*this)(n_rows - 1 - i, n_cols - 1 - j);
			//top-right
			extra_image(kernel_vert_radius - i - 1, n_cols + kernel_hor_radius + j) = (*this)(i, n_cols - 1 - j);
			//bottom-left
			extra_image(n_rows + kernel_vert_radius + i, kernel_hor_radius - j - 1) = (*this)(n_rows - 1 - i, j);
		}
	}
	return extra_image;
}
